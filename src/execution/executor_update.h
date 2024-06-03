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
    }
    std::unique_ptr<RmRecord> Next() override {
        for(auto &rid : rids_) {
            auto rec = fh_->get_record(rid, context_);
            if (rec == nullptr) {
                throw RecordNotFoundError(rid.page_no, rid.slot_no);
            }
            if(!ConditionCheck::check_conditions(conds_, tab_.cols, rec))
                continue;
            for (size_t i = 0; i < set_clauses_.size(); i++) {
                auto &col = tab_.cols[set_idxs_[i]];
                auto &val = set_clauses_[i].rhs;
                memcpy(rec->data + col.offset, val.raw->data, col.len);
            }
            fh_->update_record(rid, rec->data, context_);
        }
        return nullptr;
    }

    Rid &rid() override { return _abstract_rid; }
};