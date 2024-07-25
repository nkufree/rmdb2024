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

class InsertExecutor : public AbstractExecutor {
   private:
    TabMeta tab_;                   // 表的元数据
    std::vector<std::vector<Value>> values_;     // 需要插入的数据
    RmFileHandle *fh_;              // 表的数据文件句柄
    std::string tab_name_;          // 表名称
    Rid rid_;                       // 插入的位置，由于系统默认插入时不指定位置，因此当前rid_在插入后才赋值
    SmManager *sm_manager_;

   public:
    InsertExecutor(SmManager *sm_manager, const std::string &tab_name, std::vector<std::vector<Value>> values, Context *context) {
        sm_manager_ = sm_manager;
        tab_ = sm_manager_->db_.get_table(tab_name);
        values_ = values;
        tab_name_ = tab_name;
        for(auto &values_each : values_)
        {
            if (values_each.size() != tab_.cols.size()) {
                throw InvalidValueCountError();
            }
        }
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        context_ = context;
    };

    std::unique_ptr<RmRecord> Next() override {
        // Make record buffer
        RmRecord rec(fh_->get_file_hdr().record_size);
        char* key = new char[tab_.get_col_total_len()];
        for(auto &values : values_) {
            for (size_t i = 0; i < values.size(); i++) {
                auto &col = tab_.cols[i];
                auto &val = values[i];
                bool isMatch = (col.type == val.type || (col.type == TYPE_INT && val.type == TYPE_FLOAT) || (col.type == TYPE_FLOAT && val.type == TYPE_INT));
                if (col.type != val.type && ! isMatch) {
                    throw IncompatibleTypeError(coltype2str(col.type), coltype2str(val.type));
                }
            val.try_cast_to(col.type);

                val.init_raw(col.len);
                memcpy(rec.data + col.offset, val.raw->data, col.len);
            }
            // Insert into record file
            // 写入日志
            rid_ = fh_->get_free_record(context_);
            InsertLogRecord log_record(context_->txn_->get_transaction_id(), rec, rid_, tab_name_);
            log_record.prev_lsn_ = context_->txn_->get_prev_lsn();
            lsn_t curr_lsn = context_->log_mgr_->add_log_to_buffer(&log_record);
            context_->txn_->set_prev_lsn(curr_lsn);
            // 加锁
            context_->lock_mgr_->lock_exclusive_on_table(context_->txn_, fh_->GetFd());
            fh_->insert_record(rid_, rec.data);
            WriteRecord* wr = new WriteRecord(WType::INSERT_TUPLE, tab_name_, rid_);
            context_->txn_->append_write_record(wr);
            // Insert into index
            for(size_t i = 0; i < tab_.indexes.size(); ++i) {
                auto& index = tab_.indexes[i];
                auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                int offset = 0;
                for(size_t j = 0; j < (size_t)index.col_num; ++j) {
                    memcpy(key + offset, rec.data + index.cols[j].offset, index.cols[j].len);
                    offset += index.cols[j].len;
                }
                bool success;
                ih->insert_entry(key, rid_, context_->txn_, &success);
                if(!success) {
                    fh_->delete_record(rid_, context_);
                    for(size_t k = 0; k < i; ++k) {
                        offset = 0;
                        auto& del_index = tab_.indexes[k];
                        auto del_ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, del_index.cols)).get();
                        for(size_t j = 0; j < (size_t)del_index.col_num; ++j) {
                            memcpy(key + offset, rec.data + del_index.cols[j].offset, del_index.cols[j].len);
                            offset += del_index.cols[j].len;
                        }
                        del_ih->delete_entry(key, context_->txn_);
                    }
                    throw IndexDuplicateKeyError();
                    break;
                }
                // ih->print_tree();
            }
        }
        delete[] key;
        return nullptr;
    }
    Rid &rid() override { return rid_; }
};