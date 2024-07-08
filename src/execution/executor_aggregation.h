/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"
#include "condition_check.h"

class AggregationExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_; // 上游执行器的指针
    size_t len_;  // 每一条聚合后的记录的长度
    std::vector<Condition> having_conds_; // 保存 HAVING 子句中的条件
    std::vector<ColMeta> group_cols_; // 保存用于分组的列的元数据
    std::vector<ColMeta> sel_cols_;  // 保存被选中的列的元数据
    std::vector<ColMeta> sel_cols_origin; // 保存初始选择的列的元数据
    std::map<std::string, int> grouped_records_idx_; // 每一个组的索引
    std::vector<std::vector<std::unique_ptr<RmRecord>>> grouped_records_; // 保存每个组的记录
    int curr_idx = -1; // 当前组索引
    std::vector<std::unique_ptr<RmRecord>> curr_records; // 保存当前组的记
    bool empty_table_aggr_ = false; // 是否进行过空表聚合

   public:
    AggregationExecutor(std::unique_ptr<AbstractExecutor> prev, const std::vector<TabCol> &sel_cols, 
        const std::vector<TabCol> &group_cols, const std::vector<Condition> &having_conds) {
        prev_ = std::move(prev);

        auto &prev_cols = prev_->cols();  

        for (auto &sel_group_col : group_cols) {
            auto pos = get_col(prev_cols, sel_group_col, false);
            group_cols_.push_back(*pos);
        }

        for (auto &sel_col : sel_cols) {
            if (sel_col.aggr == ast::AGGR_TYPE_COUNT && sel_col.col_name == "*") {
                ColMeta col = star_col(sel_col);
                sel_cols_origin.push_back(col);
                sel_cols_.push_back(col);
                continue;
            }
            auto pos = get_col(prev_cols, sel_col);
            auto col = *pos;
            col.aggr = sel_col.aggr;
            sel_cols_origin.push_back(col);
            size_t p = pos - prev_cols.begin();
            if (col.aggr != ast::NO_AGGR) {
                if (sel_col.aggr == ast::AGGR_TYPE_COUNT) {
                    col.type = TYPE_INT;
                    col.len = sizeof(int);
                } else if(sel_col.aggr == ast::AGGR_TYPE_SUM) {
                    if (col.type == TYPE_STRING) {
                        col.type = TYPE_INT;
                        col.len = sizeof(int);
                    }
                }
            }

            sel_cols_.push_back(col);
        }

        size_t offset = 0;
        for (auto &col : sel_cols_) {
            col.offset = offset;
            offset += col.len;
        }
        len_ = offset;
        
        having_conds_ = having_conds;
    }

    void store_group(std::unique_ptr<RmRecord> record) {
        std::string key = "";
        for (auto group_col : group_cols_) {
            key += std::string(record->data + group_col.offset, group_col.len);
        }
        if (grouped_records_idx_.find(key) == grouped_records_idx_.end()) {
            grouped_records_idx_[key] = grouped_records_.size();
            grouped_records_.push_back(std::vector<std::unique_ptr<RmRecord>>());
        }
        grouped_records_[grouped_records_idx_[key]].push_back(std::move(record));
    }

    void beginTuple() override {
        for(auto &cond : having_conds_) {
            ConditionCheck::execute_sub_query(cond);
        }
        for (prev_->beginTuple(); !prev_->is_end(); prev_->nextTuple()) {
            auto record = prev_->Next();
            store_group(std::move(record));
        }
        nextTuple();
    }

    void nextTuple() override {
        do {
            if (grouped_records_.empty() && group_cols_.empty()) {
                if (!empty_table_aggr_) {
                    empty_table_aggr_ = true;
                    return;
                }
            }
            ++curr_idx;
            if (is_end()) return;
            auto &grouped_records = grouped_records_[curr_idx];
            curr_records.clear();
            for (auto &record : grouped_records) {
                curr_records.push_back(std::move(record));
            }
        } while (!checkConditions());
    }

    bool checkConditions() {
        std::vector<int> to_delete;
        int i = 0;
        for (auto &record : curr_records) {
            auto handle = record.get();
            char *base = handle->data;
            if (!std::all_of(having_conds_.begin(), having_conds_.end(), [base, this](const Condition& cond) {
                if(cond.lhs_col.aggr == ast::NO_AGGR) {
                    auto value = get_col(sel_cols_origin, cond.lhs_col, true)->to_value(base);
                    return cond.eval_with_rvalue(value);
                } else {
                    ColMeta col_meta;
                    if (cond.lhs_col.aggr == ast::AGGR_TYPE_COUNT && cond.lhs_col.col_name == "*") {
                        col_meta = star_col(cond.lhs_col);
                    } else {
                        col_meta = *get_col(sel_cols_origin, cond.lhs_col, true);
                    }
                    auto value = aggregate_value(col_meta);
                    Value rhs_val = cond.rhs_val;
                    if(value.type != rhs_val.type)
                    {
                        if(value.type == ColType::TYPE_INT && rhs_val.type == ColType::TYPE_FLOAT)
                        {
                            value.set_float(value.int_val);
                            // rhs_val.set_int(rhs_val.float_val);
                        }
                        else if(value.type == ColType::TYPE_FLOAT && rhs_val.type == ColType::TYPE_INT)
                        {
                            rhs_val.set_float(rhs_val.int_val);
                        }
                        else
                        {
                            throw IncompatibleTypeError(coltype2str(value.type), coltype2str(rhs_val.type));
                        }
                    }
                    return cond.check_condition(value, rhs_val);
                }
            })) {
                to_delete.push_back(i);
            }
            ++i;
        }
        if (to_delete.size() == curr_records.size()) return false;
        for (int i = to_delete.size() - 1; i >= 0; --i) {
            curr_records.erase(curr_records.begin() + to_delete[i]);
        }
        return true;
    }

    ColMeta star_col(TabCol c) {
        ColMeta col;
        col.name = "*";
        col.tab_name = "";
        col.alias = c.alias;
        col.type = TYPE_INT;
        col.len = sizeof(int);
        col.offset = 0;
        col.aggr = ast::AGGR_TYPE_COUNT;
        return col;
    }

    Value aggregate_value(ColMeta sel_col) {
        Value val;
        if (curr_records.empty() && sel_col.aggr != ast::AGGR_TYPE_COUNT && sel_col.aggr != ast::AGGR_TYPE_SUM) {
            val.type = sel_col.type;
            val.init_raw(sel_col.len);
            val.type = TYPE_NULL;
            return val;
        }
        switch (sel_col.aggr) {
            case ast::NO_AGGR:
                val = sel_col.to_value(curr_records[0]->data);
                val.init_raw(sel_col.len);
                break;
            case ast::AGGR_TYPE_COUNT:
                val.type = TYPE_INT;
                val.set_int(curr_records.size());
                val.init_raw(sizeof(int));
                break;
            case ast::AGGR_TYPE_MAX:
                val.type = sel_col.type;
                val = sel_col.to_value(curr_records[0]->data);
                for (auto &record : curr_records) {
                    Value tmp = sel_col.to_value(record->data);
                    if (tmp > val) val = tmp;
                }
                val.init_raw(sel_col.len);
                break;
            case ast::AGGR_TYPE_MIN:
                val.type = sel_col.type;
                val = sel_col.to_value(curr_records[0]->data);
                for (auto &record : curr_records) {
                    Value tmp = sel_col.to_value(record->data);
                    if (tmp < val) val = tmp;
                }
                val.init_raw(sel_col.len);
                break;
            case ast::AGGR_TYPE_SUM:
                val.type = sel_col.type;
                if (sel_col.type == TYPE_INT) {
                    int sum = 0;
                    for (auto &record : curr_records) {
                        sum += *(int *)(record->data + sel_col.offset);
                    }
                    val.set_int(sum);
                    val.init_raw(sizeof(int));
                } else if (sel_col.type == TYPE_FLOAT) {
                    float sum = 0;
                    for (auto &record : curr_records) {
                        sum += *(float *)(record->data + sel_col.offset);
                    }
                    val.set_float(sum);
                    val.init_raw(sizeof(float));
                } else if (sel_col.type == TYPE_STRING) {
                    val.type = TYPE_INT;
                    std::string sum;
                    float sum_float = 0;
                    bool is_float = false;

                    for (auto &record : curr_records) {
                        const char* base = record->data + sel_col.offset;
                        sum = "";
                        for (size_t i=0; i<sel_col.len; ++i) {
                            if (base[i] >= '0' && base[i] <= '9') sum += base[i];
                            else if (base[i] == '.'){
                                sum += base[i];
                                is_float = true;
                            }
                            else break;
                        }
                        if (sum.size() == 0) continue;
                        sum_float += std::stof(sum);
                    }
                    if (is_float) {
                        val.set_float(sum_float);
                        val.type = TYPE_FLOAT;
                        val.init_raw(sizeof(float));
                    } else {
                        val.set_int((int)sum_float);
                        val.init_raw(sizeof(int));
                    }
                } else {
                    throw InternalError("Unknown AggrType");
                }
                break;
            
            default:
                throw InternalError("Unknown AggrType");
        }
        return val;
    }

    bool is_end() const override{
        return curr_idx >= (int)grouped_records_.size();
    }

    const std::vector<ColMeta> &cols() const override{
        return sel_cols_;
    }

    std::unique_ptr<RmRecord> Next() override {
        std::vector<Value> values;

        int idx = 0;
        for (auto sel_col : sel_cols_origin) {
            Value val = aggregate_value(sel_col);
            if (val.type == TYPE_NULL) {
                sel_cols_[idx].type = TYPE_NULL;
            }
            values.push_back(val);
            ++idx;
        }

        auto data = std::make_unique<char[]>(len_);
        size_t offset = 0;
        size_t i = 0;
        for (auto col : sel_cols_) {
            memcpy(data.get() + offset, values[i].raw->data, values[i].raw->size);
            ++i;
            offset += col.len;
        }
        return std::make_unique<RmRecord>(len_, data.get());
    }

    Rid &rid() override { return _abstract_rid; }

    ExecutorType getType() override {
        return ExecutorType::AGGREGATION_EXECUTOR;
    }
};