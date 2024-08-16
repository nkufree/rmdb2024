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
        // conds_ = conds;
        conds_ = std::vector<Condition>();
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
        for(auto &cond : conds_) {
            ConditionCheck::execute_sub_query(cond);
        }
        // 先检查是否存在，存在则抛出异常
        // for(auto &rid : rids_) {
        //     auto rec = fh_->get_record(rid, context_);
        //     for (size_t i = 0; i < set_clauses_.size(); i++) {
        //         auto &col = tab_.cols[set_idxs_[i]];
        //         auto &val = set_clauses_[i].rhs;
        //         memcpy(rec->data + col.offset, val.raw->data, col.len);
        //     }
        //     for(auto& index: update_indexes_) {
        //         auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
        //         char* key = new char[index.col_tot_len];
        //         int offset = 0;
        //         for(size_t i = 0; i < (size_t)index.col_num; ++i) {
        //             memcpy(key + offset, rec->data + index.cols[i].offset, index.cols[i].len);
        //             offset += index.cols[i].len;
        //         }
        //         Iid lower = ih->lower_bound(key);
        //         Iid upper = ih->upper_bound(key);
        //         if(lower != upper) {
        //             Rid curr = ih->get_rid(lower);
        //             if(!(curr == rid)) {
        //                 delete[] key;
        //                 throw IndexDuplicateKeyError();
        //             }
        //         }
        //         delete[] key;
        //     }
        // }
        char* key = new char[tab_.get_col_total_len()];
        for(auto &rid : rids_) {
            auto rec = fh_->get_record(rid, context_);
            std::unique_ptr<RmRecord> rec_new(new RmRecord(*rec));
            for (size_t i = 0; i < set_clauses_.size(); i++) {
                auto &col = tab_.cols[set_idxs_[i]];
                auto &val = set_clauses_[i].rhs;
                memcpy(rec_new->data + col.offset, val.raw->data, col.len);
            }
            if (rec == nullptr) {
                delete[] key;
                throw RecordNotFoundError(rid.page_no, rid.slot_no);
            }
            // if(!ConditionCheck::check_conditions(conds_, rec))
            //     continue;
            // 更新记录
            UpdateLogRecord log_record(context_->txn_->get_transaction_id(), *rec, *rec_new, rid, tab_name_);
            log_record.prev_lsn_ = context_->txn_->get_prev_lsn();
            lsn_t curr_lsn = context_->log_mgr_->add_log_to_buffer(&log_record);
            context_->txn_->set_prev_lsn(curr_lsn);
            fh_->update_record(rid, rec_new->data, context_);
            WriteRecord* wr = new WriteRecord(WType::UPDATE_TUPLE, tab_name_, rid, *rec);
            context_->txn_->append_write_record(wr);
            // 更新索引
            // 先尝试插入数据，如果插入成功再尝试删除原来的数据
            // int failed_idx = -1;
            for(auto& index : update_indexes_) {
                // auto& index = update_indexes_[i];
                auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                int offset = 0;
                for(size_t i = 0; i < (size_t)index.col_num; ++i) {
                    memcpy(key + offset, rec_new->data + index.cols[i].offset, index.cols[i].len);
                    offset += index.cols[i].len;
                }
                bool success;
                ih->insert_entry(key, rid, context_->txn_, &success);
                if(!success) {
                    delete[] key;
                    throw TransactionAbortException(context_->txn_->get_transaction_id(), AbortReason::UPGRADE_CONFLICT);
                    // continue;
                }
                offset = 0;
                for(size_t i = 0; i < (size_t)index.col_num; ++i) {
                    memcpy(key + offset, rec->data + index.cols[i].offset, index.cols[i].len);
                    offset += index.cols[i].len;
                }
                ih->delete_entry(key, context_->txn_);
            }
            // if(failed_idx != -1) {
            //     // 如果失败，删除之前插入的数据
            //     for(int i = failed_idx - 1; i >= 0; i--) {
            //         auto& index = update_indexes_[i];
            //         auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
            //         int offset = 0;
            //         for(size_t i = 0; i < (size_t)index.col_num; ++i) {
            //             memcpy(key + offset, rec_new->data + index.cols[i].offset, index.cols[i].len);
            //             offset += index.cols[i].len;
            //         }
            //         ih->delete_entry(key, context_->txn_);
            //         // TODO: 回滚之前更新的数据
            //         throw IndexDuplicateKeyError();
            //     }
            // }
        }
        delete[] key;
        return nullptr;
    }

    Rid &rid() override { return _abstract_rid; }
};