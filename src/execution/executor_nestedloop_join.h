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

class NestedLoopJoinExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> left_;    // 左儿子节点（需要join的表）
    std::unique_ptr<AbstractExecutor> right_;   // 右儿子节点（需要join的表）
    size_t len_;                                // join后获得的每条记录的长度
    std::vector<ColMeta> cols_;                 // join后获得的记录的字段

    std::vector<Condition> fed_conds_;          // join条件
    bool isend;

   public:
    NestedLoopJoinExecutor(std::unique_ptr<AbstractExecutor> left, std::unique_ptr<AbstractExecutor> right, 
                            std::vector<Condition> conds) {
        left_ = std::move(left);
        right_ = std::move(right);
        len_ = left_->tupleLen() + right_->tupleLen();
        cols_ = left_->cols();
        auto right_cols = right_->cols();
        for (auto &col : right_cols) {
            col.offset += left_->tupleLen();
        }

        cols_.insert(cols_.end(), right_cols.begin(), right_cols.end());
        isend = false;
        fed_conds_ = std::move(conds);
        for(auto &cond : fed_conds_) {
            cond.lhs_match_col = get_col(cols_, cond.lhs_col);
            if(cond.rhs_type == CondRhsType::RHS_COL) {
                cond.rhs_match_col = get_col(cols_, cond.rhs_col);
            }
            else if(cond.rhs_type == CondRhsType::RHS_VALUE) {
                cond.rhs_val.value_cast(cond.lhs_match_col->type);
            }
        }
    }

    void beginTuple() override {
        isend = false;
        left_->beginTuple();
        while(!(left_->is_end())) {
            right_->beginTuple();
            while(!(right_->is_end())) {
                auto rec_left = left_->Next();
                auto rec_right = right_->Next();
                std::unique_ptr<RmRecord> new_rec = std::make_unique<RmRecord>(len_);
                memcpy(new_rec->data, rec_left->data, left_->tupleLen());
                memcpy(new_rec->data + left_->tupleLen(), rec_right->data, right_->tupleLen());
                if(ConditionCheck::check_conditions(fed_conds_, new_rec)) {
                    return;
                }
                right_->nextTuple();
            }
            left_->nextTuple();
        }
        isend = true;
    }

    void nextTuple() override {
        right_->nextTuple();
        while(!(left_->is_end())) {
            while(!(right_->is_end())) {
                auto rec_left = left_->Next();
                auto rec_right = right_->Next();
                std::unique_ptr<RmRecord> new_rec = std::make_unique<RmRecord>(len_);
                memcpy(new_rec->data, rec_left->data, left_->tupleLen());
                memcpy(new_rec->data + left_->tupleLen(), rec_right->data, right_->tupleLen());
                if(ConditionCheck::check_conditions(fed_conds_, new_rec)) {
                    return;
                }
                right_->nextTuple();
            }
            left_->nextTuple();
            right_->beginTuple();
        }
        isend = true;
    }

    std::unique_ptr<RmRecord> Next() override {
        auto rec_left = left_->Next();
        auto rec_right = right_->Next();
        std::unique_ptr<RmRecord> new_rec = std::make_unique<RmRecord>(len_);
        memcpy(new_rec->data, rec_left->data, left_->tupleLen());
        memcpy(new_rec->data + left_->tupleLen(), rec_right->data, right_->tupleLen());
        return new_rec;
    }

    bool is_end() const override { return isend; }

    const std::vector<ColMeta> &cols() const override { return cols_; }

    size_t tupleLen() const override { return len_; }

    Rid &rid() override { return _abstract_rid; }
};