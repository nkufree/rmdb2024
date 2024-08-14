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

class DeleteExecutor : public AbstractExecutor {
   private:
    TabMeta tab_;                   // 表的元数据
    std::vector<Condition> conds_;  // delete的条件
    RmFileHandle *fh_;              // 表的数据文件句柄
    std::vector<Rid> rids_;         // 需要删除的记录的位置
    std::string tab_name_;          // 表名称
    SmManager *sm_manager_;

   public:
    DeleteExecutor(SmManager *sm_manager, const std::string &tab_name, std::vector<Condition> conds,
                   std::vector<Rid> rids, Context *context) {
        sm_manager_ = sm_manager;
        tab_name_ = tab_name;
        tab_ = sm_manager_->db_.get_table(tab_name);
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        // conds_ = conds;
        conds_ = std::vector<Condition>();
        rids_ = rids;
        context_ = context;
    }

    std::unique_ptr<RmRecord> Next() override {
        // for(auto &cond : conds_) {
        //     ConditionCheck::execute_sub_query(cond);
        // }
        char* key = new char[tab_.get_col_total_len()];
        for(auto &rid : rids_) {
            auto rec = fh_->get_record(rid, context_);
            if (rec == nullptr) {
                throw RecordNotFoundError(rid.page_no, rid.slot_no);
            }
            // if(!ConditionCheck::check_conditions(conds_, tab_.cols, rec))
            //     continue;
            DeleteLogRecord log_record(context_->txn_->get_transaction_id(), *rec, rid, tab_name_);
            log_record.prev_lsn_ = context_->txn_->get_prev_lsn();
            lsn_t curr_lsn = context_->log_mgr_->add_log_to_buffer(&log_record);
            context_->txn_->set_prev_lsn(curr_lsn);
            fh_->delete_record(rid, context_);
            WriteRecord* wr = new WriteRecord(WType::DELETE_TUPLE, tab_name_, rid, *rec.get());
            context_->txn_->append_write_record(wr);
            for(size_t i = 0; i < tab_.indexes.size(); ++i) {
                auto& index = tab_.indexes[i];
                auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                int offset = 0;
                for(size_t i = 0; i < (size_t)index.col_num; ++i) {
                    memcpy(key + offset, rec->data + index.cols[i].offset, index.cols[i].len);
                    offset += index.cols[i].len;
                }
                ih->delete_entry(key, context_->txn_);
            }
        }
        delete[] key;
        return nullptr;
    }

    Rid &rid() override { return _abstract_rid; }
};