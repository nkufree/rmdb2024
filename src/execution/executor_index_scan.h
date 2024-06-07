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
#include <limits>

struct ColRange {
    bool has_lower_bound;
    Value lower_val;
    bool has_upper_bound;
    Value upper_val;
    ColRange(){
        has_lower_bound = false;
        has_upper_bound = false;
    }
};

class IndexScanExecutor : public AbstractExecutor {
   private:
    std::string tab_name_;                      // 表名称
    TabMeta tab_;                               // 表的元数据
    std::vector<Condition> conds_;              // 扫描条件
    RmFileHandle *fh_;                          // 表的数据文件句柄
    std::vector<ColMeta> cols_;                 // 需要读取的字段
    size_t len_;                                // 选取出来的一条记录的长度
    std::vector<Condition> fed_conds_;          // 扫描条件，和conds_字段相同

    std::vector<std::string> index_col_names_;  // index scan涉及到的索引包含的字段
    IndexMeta index_meta_;                      // index scan涉及到的索引元数据

    Rid rid_;
    std::unique_ptr<RecScan> scan_;

    SmManager *sm_manager_;
    int equal_col_num;                          // 等值且在索引中连续的条件的个数
    std::vector<ColRange> other_col_range;          // 不在等于范围内的条件的范围
   public:
    IndexScanExecutor(SmManager *sm_manager, std::string tab_name, std::vector<Condition> conds, std::vector<std::string> index_col_names,
                    Context *context) {
        sm_manager_ = sm_manager;
        context_ = context;
        tab_name_ = std::move(tab_name);
        tab_ = sm_manager_->db_.get_table(tab_name_);
        conds_ = std::move(conds);
        // index_no_ = index_no;
        index_col_names_ = index_col_names; 
        index_meta_ = *(tab_.get_index_meta(index_col_names_));
        fh_ = sm_manager_->fhs_.at(tab_name_).get();
        cols_ = tab_.cols;
        len_ = cols_.back().offset + cols_.back().len;
        std::map<CompOp, CompOp> swap_op = {
            {OP_EQ, OP_EQ}, {OP_NE, OP_NE}, {OP_LT, OP_GT}, {OP_GT, OP_LT}, {OP_LE, OP_GE}, {OP_GE, OP_LE},
        };
        // 构建列名到索引下标的映射
        std::unordered_map<std::string, int> col_name_to_index;
        for (int i = 0; i < (int)index_meta_.col_num; i++) {
            col_name_to_index[index_meta_.cols[i].name] = i;
        }
        fed_conds_.clear();
        other_col_range.resize(index_meta_.col_num);
        equal_col_num = 0;
        bool still_equal = true;
        for (size_t i = 0; i < conds_.size(); i++) {
            auto &cond = conds_[i];
            if (cond.lhs_col.tab_name != tab_name_) {
                // lhs is on other table, now rhs must be on this table
                assert(!cond.is_rhs_val && cond.rhs_col.tab_name == tab_name_);
                // swap lhs and rhs
                std::swap(cond.lhs_col, cond.rhs_col);
                cond.op = swap_op.at(cond.op);
            }
            if(cond.op == OP_EQ && still_equal && (int)i < index_meta_.col_num && cond.is_rhs_val && conds_[i].lhs_col.col_name == index_col_names_[equal_col_num].c_str()) {
                equal_col_num++;
            }
            else {
                still_equal = false;
                fed_conds_.push_back(cond);
                if(!cond.is_rhs_val) 
                    continue;
                if(col_name_to_index.find(cond.lhs_col.col_name) == col_name_to_index.end())
                    continue;
                ColRange& other_col = other_col_range[col_name_to_index[cond.lhs_col.col_name]];
                switch (cond.op)
                {
                case OP_GE:
                case OP_GT:
                    other_col.has_lower_bound = true;
                    other_col.lower_val = cond.rhs_val;
                    break;
                case OP_LE:
                case OP_LT:
                    other_col.has_upper_bound = true;
                    other_col.upper_val = cond.rhs_val;
                    break;
                default:
                    break;
                }
            }
        }
        // fed_conds_ = conds_;
    }

    void beginTuple() override {
        // std::cout << "use index" << std::endl;
        IxIndexHandle* ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index_col_names_)).get();
        // 遍历检索条件，找出索引条件的下限和上限
        char* key = new char[index_meta_.col_tot_len];
        int offset = build_equal_key(key);
        build_lower_key(key, offset);
        build_range_key(key, offset, true);
        Iid lower = ih->lower_bound(key);
        build_upper_key(key, offset);
        build_range_key(key, offset, false);
        Iid upper = ih->upper_bound(key);
        delete[] key;
        // ih->print_tree();
        // std::cout << "lower: " << lower.page_no << " " << lower.slot_no << std::endl;
        // std::cout << "upper: " << upper.page_no << " " << upper.slot_no << std::endl;
        scan_ = std::make_unique<IxScan>(ih, lower, upper, sm_manager_->get_bpm());
        // 遍历获取第一个元素
        std::unique_ptr<RmRecord> rec;
        while (!scan_->is_end())
        {
            rid_ = scan_->rid();
            rec = fh_->get_record(rid_, context_);
            if(ConditionCheck::check_conditions(fed_conds_, cols_, rec))
                break;
            scan_->next();
        }
    }

    void nextTuple() override {
        std::unique_ptr<RmRecord> rec;
        while (true)
        {
            scan_->next();
            if(scan_->is_end())
                break;
            rid_ = scan_->rid();
            rec = fh_->get_record(rid_, context_);
            if(ConditionCheck::check_conditions(fed_conds_, cols_, rec))
                break;
        }
        
    }

    std::unique_ptr<RmRecord> Next() override {
        return fh_->get_record(rid_, context_);
    }

    Rid &rid() override { return rid_; }

    bool is_end() const override { return scan_->is_end(); }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta> &cols() const override { return cols_; }

private:
    int build_equal_key(char* key) {
        int offset = 0;
        for(int i = 0; i < equal_col_num; i++) {
            conds_[i].rhs_val.value_cast(index_meta_.cols[i].type);
            switch (index_meta_.cols[i].type)
            {
            case TYPE_INT:
                memcpy(key + offset, (void*)&conds_[i].rhs_val.int_val, sizeof(int));
                break;
            case TYPE_FLOAT:
                memcpy(key + offset, (void*)&conds_[i].rhs_val.float_val, sizeof(float));
                break;
            case TYPE_STRING:
                memset(key + offset, 0, index_meta_.cols[i].len);
                memcpy(key + offset, conds_[i].rhs_val.str_val.c_str(), conds_[i].rhs_val.str_val.size());
                break;
            default:
                break;
            }
            offset += index_meta_.cols[i].len;
        }
        int start = offset;
        for(int i = equal_col_num; i < (int)index_meta_.col_num; i++) {
            switch (index_meta_.cols[i].type)
            {
            case TYPE_INT:
                *(int*)(key + start) = std::numeric_limits<int>::min();
                break;
            case TYPE_FLOAT:
                *(float*)(key + start) = std::numeric_limits<float>::min();
                break;
            case TYPE_STRING:
                memset(key + start, 0, index_meta_.cols[i].len);
                break;
            default:
                break;
            }
            start += index_meta_.cols[i].len;
        }
        return offset;
    }

    void build_lower_key(char* key, int offset) {
        for(int i = equal_col_num; i < (int)index_meta_.col_num; i++) {
            switch (index_meta_.cols[i].type)
            {
            case TYPE_INT:
                *(int*)(key + offset) = std::numeric_limits<int>::min();
                break;
            case TYPE_FLOAT:
                *(float*)(key + offset) = std::numeric_limits<float>::min();
                break;
            case TYPE_STRING:
                memset(key + offset, 0, index_meta_.cols[i].len);
                break;
            default:
                break;
            }
            offset += index_meta_.cols[i].len;
        }
    }

    void build_upper_key(char* key, int offset) {
        for(int i = equal_col_num; i < (int)index_meta_.col_num; i++) {
            switch (index_meta_.cols[i].type)
            {
            case TYPE_INT:
                *(int*)(key + offset) = std::numeric_limits<int>::max();
                break;
            case TYPE_FLOAT:
                *(float*)(key + offset) = std::numeric_limits<float>::max();
                break;
            case TYPE_STRING:
                memset(key + offset, 0xFF, index_meta_.cols[i].len);
                break;
            default:
                break;
            }
            offset += index_meta_.cols[i].len;
        }
    }

    void build_range_key(char* key, int start, bool is_lower_bound) {
        int offset = start;
        for(int i = equal_col_num; i < (int)index_meta_.col_num; i++) {
            bool need_fill = is_lower_bound ? other_col_range[i].has_lower_bound : other_col_range[i].has_upper_bound;
            Value& val = is_lower_bound ? other_col_range[i].lower_val : other_col_range[i].upper_val;
            if(!need_fill) {
                offset += index_meta_.cols[i].len;
                continue;
            }
            val.value_cast(index_meta_.cols[i].type);
            switch (index_meta_.cols[i].type)
            {
            case TYPE_INT:
                memcpy(key + offset, (void*)&val.int_val, sizeof(int));
                break;
            case TYPE_FLOAT:
                memcpy(key + offset, (void*)&val.float_val, sizeof(float));
                break;
            case TYPE_STRING:
                memset(key + offset, 0, index_meta_.cols[i].len);
                memcpy(key + offset, val.str_val.c_str(), val.str_val.size());
                break;
            default:
                break;
            }
            offset += index_meta_.cols[i].len;
        }
    }
};