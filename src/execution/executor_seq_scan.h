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

class SeqScanExecutor : public AbstractExecutor {
   private:
    std::string tab_name_;              // 表的名称
    std::vector<Condition> conds_;      // scan的条件
    RmFileHandle *fh_;                  // 表的数据文件句柄
    std::vector<ColMeta> cols_;         // scan后生成的记录的字段
    size_t len_;                        // scan后生成的每条记录的长度
    std::vector<Condition> fed_conds_;  // 同conds_，两个字段相同

    Rid rid_;
    std::unique_ptr<RecScan> scan_;     // table_iterator

    SmManager *sm_manager_;
    bool read_only_;

   public:
    SeqScanExecutor(SmManager *sm_manager, std::string tab_name, std::vector<Condition> conds, Context *context, bool read_only=true) {
        sm_manager_ = sm_manager;
        tab_name_ = std::move(tab_name);
        conds_ = std::move(conds);
        TabMeta &tab = sm_manager_->db_.get_table(tab_name_);
        fh_ = sm_manager_->fhs_.at(tab_name_).get();
        cols_ = tab.cols;
        len_ = cols_.back().offset + cols_.back().len;

        context_ = context;
        read_only_ = read_only;
        fed_conds_ = conds_;
        for(auto &cond : fed_conds_) {
            ConditionCheck::execute_sub_query(cond);
        }
    }

    void beginTuple() override {
        // 对没加锁的表加锁
        {
            auto lock_set = context_->txn_->get_lock_set();
            int table_fd = fh_->GetFd();
            if(lock_set->find(LockDataId(table_fd, LockDataType::TABLE)) == lock_set->end()) {
                if(read_only_) {
                    context_->lock_mgr_->lock_shared_on_table(context_->txn_, table_fd);
                } else {
                    context_->lock_mgr_->lock_exclusive_on_table(context_->txn_, table_fd);
                }
            }
        }
        scan_ = std::make_unique<RmScan>(fh_);
        // rid_ = scan_->rid();
        std::unique_ptr<RmRecord> rec;
        while (!scan_->is_end())
        {
            rec = fh_->get_record(scan_->rid(), context_);
            if(ConditionCheck::check_conditions(fed_conds_, cols_, rec))
                break;
            scan_->next();
        }
        rid_ = scan_->rid();
    }

    void nextTuple() override {
        // scan_->next();
        // rid_ = scan_->rid();
        std::unique_ptr<RmRecord> rec;
        scan_->next();
        while (!scan_->is_end())
        {
            rec = fh_->get_record(scan_->rid(), context_);
            if(ConditionCheck::check_conditions(fed_conds_, cols_, rec))
            {
                break;
            }
            scan_->next();
        }
        rid_ = scan_->rid();
    }

    std::unique_ptr<RmRecord> Next() override {
        return fh_->get_record(rid_, context_);
    }

    Rid &rid() override { return rid_; }

    bool is_end() const override { return scan_->is_end(); }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta> &cols() const override { return cols_; }
};