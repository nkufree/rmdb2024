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

class UpdateExecutor : public AbstractExecutor {
   private:
    TabMeta tab_;
    std::vector<Condition> conds_;
    RmFileHandle *fh_;
    std::vector<Rid> rids_;
    std::string tab_name_;
    std::vector<SetClause> set_clauses_;
    SmManager *sm_manager_;
    std::vector<size_t> set_idxs_;
    std::vector<IndexMeta> update_indexes_;
   public:
    UpdateExecutor(SmManager *sm_manager, const std::string &tab_name, std::vector<SetClause> set_clauses,
                   std::vector<Condition> conds, std::vector<Rid> rids, Context *context) {
        sm_manager_ = sm_manager;
        tab_name_ = tab_name;
        set_clauses_ = set_clauses;
        tab_ = sm_manager_->db_.get_table(tab_name);
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        conds_ = conds;
        rids_ = rids;
        context_ = context;
        for(size_t i = 0; i < set_clauses_.size(); i++) {
            auto pos = get_col(tab_.cols, set_clauses_[i].lhs);
            set_idxs_.push_back(pos - tab_.cols.begin());
            if (pos->type != set_clauses_[i].rhs.type) {
                set_clauses_[i].rhs.value_cast(pos->type);
            }
            set_clauses_[i].rhs.init_raw(pos->len);
        }
        for(auto &idx : tab_.indexes) {
            for(auto &col : idx.cols) {
                for(auto &set_clause : set_clauses_) {
                    if(set_clause.lhs.col_name == col.name) {
                        update_indexes_.push_back(idx);
                        break;
                    }
                }
            }
        }
    }
    std::unique_ptr<RmRecord> Next() override {
        char* key = new char[tab_.get_col_total_len()];
        for(auto &rid : rids_) {
            auto rec = fh_->get_record(rid, context_);
            if (rec == nullptr) {
                throw RecordNotFoundError(rid.page_no, rid.slot_no);
            }
            if(!ConditionCheck::check_conditions(conds_, tab_.cols, rec))
                continue;
            // 从索引中删除旧的记录
            for(auto& index: update_indexes_) {
                auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                int offset = 0;
                for(size_t i = 0; i < (size_t)index.col_num; ++i) {
                    memcpy(key + offset, rec->data + index.cols[i].offset, index.cols[i].len);
                    offset += index.cols[i].len;
                }
                ih->delete_entry(key, context_->txn_);
            }
            for (size_t i = 0; i < set_clauses_.size(); i++) {
                auto &col = tab_.cols[set_idxs_[i]];
                auto &val = set_clauses_[i].rhs;
                memcpy(rec->data + col.offset, val.raw->data, col.len);
            }
            // 将更新过后的记录插入索引
            bool update = true;
            for(auto& index: update_indexes_) {
                auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                int offset = 0;
                for(size_t i = 0; i < (size_t)index.col_num; ++i) {
                    memcpy(key + offset, rec->data + index.cols[i].offset, index.cols[i].len);
                    offset += index.cols[i].len;
                }
                bool success;
                ih->insert_entry(key, rid, context_->txn_, &success);
                if(!success) {
                    update = false;
                    break;
                }
            }
            if(update)
                fh_->update_record(rid, rec->data, context_);
        }
        delete[] key;
        return nullptr;
    }

    Rid &rid() override { return _abstract_rid; }
};