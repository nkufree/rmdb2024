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
        fed_conds_.clear();
        equal_col_num = 0;
        bool still_equal = true;
        for (auto &cond : conds_) {
            if (cond.lhs_col.tab_name != tab_name_) {
                // lhs is on other table, now rhs must be on this table
                assert(!cond.is_rhs_val && cond.rhs_col.tab_name == tab_name_);
                // swap lhs and rhs
                std::swap(cond.lhs_col, cond.rhs_col);
                cond.op = swap_op.at(cond.op);
            }
            if(cond.op == OP_EQ && still_equal) {
                equal_col_num++;
            }
            else {
                still_equal = false;
                fed_conds_.push_back(cond);
            }
        }
        // fed_conds_ = conds_;
    }

    void beginTuple() override {
        // std::cout << "use index" << std::endl;
        IxIndexHandle* ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index_col_names_)).get();
        // 遍历检索条件，找出索引条件的下限和上限
        // TODO(zzx)：还可以根据条件进一步限定范围
        char* key = new char[index_meta_.col_tot_len];
        build_equal_key(key);
        Iid lower = ih->lower_bound(key);
        Iid upper = ih->upper_bound(key);
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

    void build_range_key(char* key, int start, bool is_lower_bound) {
        // TODO(zzx): 待完善
        int offset = start;
        for(int i = equal_col_num; i < (int)index_meta_.col_num; i++) {
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
    }
};