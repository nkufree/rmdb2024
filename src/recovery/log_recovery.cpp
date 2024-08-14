/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "log_recovery.h"

/**
 * @description: analyze阶段，需要获得脏页表（DPT）和未完成的事务列表（ATT）
 */
void RecoveryManager::analyze() {
    last_checkpoint_ = disk_manager_->get_restart_log();
    int offset = last_checkpoint_;
    LogRecord base_record;
    int buffer_size = disk_manager_->read_log(buffer_.buffer_, LOG_BUFFER_SIZE, offset);
    while(true)
    {
        if(buffer_.offset_ + OFFSET_LOG_DATA > buffer_size)
        {
            buffer_size = disk_manager_->read_log(buffer_.buffer_, LOG_BUFFER_SIZE, offset);
            if (buffer_size < OFFSET_LOG_DATA)
            {
                break;
            }
            buffer_.offset_ = 0;
        }
        base_record.deserialize(buffer_.buffer_ + buffer_.offset_);
        if(buffer_.offset_ + base_record.log_tot_len_ > buffer_size)
        {
            buffer_size = disk_manager_->read_log(buffer_.buffer_, LOG_BUFFER_SIZE, offset);
            if (buffer_size < base_record.log_tot_len_)
            {
                break;
            }
            buffer_.offset_ = 0;
        }
        offset += base_record.log_tot_len_;
        if(base_record.log_type_ == LogType::STATIC_CHECKPOINT) {
            std::shared_ptr<StaticCKPTLogRecord> log_record = std::make_shared<StaticCKPTLogRecord>();
            log_record->deserialize(buffer_.buffer_ + buffer_.offset_);
            log_records_.emplace(base_record.lsn_, log_record);
        } else if(base_record.log_type_ == LogType::begin) {
            std::shared_ptr<BeginLogRecord> log_record = std::make_shared<BeginLogRecord>();
            log_record->deserialize(buffer_.buffer_ + buffer_.offset_);
            log_records_.emplace(base_record.lsn_, log_record);
            undo_txn_.emplace(log_record->log_tid_, base_record.lsn_);
            txn_list_.push_back(log_record->log_tid_);
        } else if(base_record.log_type_ == LogType::commit) {
            std::shared_ptr<CommitLogRecord> log_record = std::make_shared<CommitLogRecord>();
            log_record->deserialize(buffer_.buffer_ + buffer_.offset_);
            log_records_.emplace(base_record.lsn_, log_record);
            redo_txn_.emplace(log_record->log_tid_, base_record.lsn_);
            undo_txn_.erase(log_record->log_tid_);
        } else if(base_record.log_type_ == LogType::ABORT) {
            std::shared_ptr<AbortLogRecord> log_record = std::make_shared<AbortLogRecord>();
            log_record->deserialize(buffer_.buffer_ + buffer_.offset_);
            log_records_.emplace(base_record.lsn_, log_record);
            undo_txn_[log_record->log_tid_] = base_record.lsn_;
        } else if(base_record.log_type_ == LogType::UPDATE) {
            std::shared_ptr<UpdateLogRecord> log_record = std::make_shared<UpdateLogRecord>();
            log_record->deserialize(buffer_.buffer_ + buffer_.offset_);
            log_records_.emplace(base_record.lsn_, log_record);
            undo_txn_[log_record->log_tid_] = base_record.lsn_;
        } else if(base_record.log_type_ == LogType::INSERT) {
            std::shared_ptr<InsertLogRecord> log_record = std::make_shared<InsertLogRecord>();
            log_record->deserialize(buffer_.buffer_ + buffer_.offset_);
            log_records_.emplace(base_record.lsn_, log_record);
            undo_txn_[log_record->log_tid_] = base_record.lsn_;
        } else if(base_record.log_type_ == LogType::DELETE) {
            std::shared_ptr<DeleteLogRecord> log_record = std::make_shared<DeleteLogRecord>();
            log_record->deserialize(buffer_.buffer_ + buffer_.offset_);
            log_records_.emplace(base_record.lsn_, log_record);
            undo_txn_[log_record->log_tid_] = base_record.lsn_;
        }
        else {
            throw InternalError("Invalid log type");
        }
        buffer_.offset_ += base_record.log_tot_len_;
    }
    // 将事务执行的操作按顺序分配到各个页面上
    std::map<lsn_t, std::shared_ptr<LogRecord>>::iterator it = log_records_.begin();
    for(;it != log_records_.end(); it++)
    {
        auto& log_record = it->second;
        if(log_record->log_type_ == LogType::begin || log_record->log_type_ == LogType::commit || log_record->log_type_ == LogType::ABORT)
        {
            continue;
        }
        page_id_t page_id;
        std::string table_name;
        if(log_record->log_type_ == LogType::UPDATE)
        {
            std::shared_ptr<UpdateLogRecord> update_log_record = std::dynamic_pointer_cast<UpdateLogRecord>(log_record);
            page_id = update_log_record->rid_.page_no;
            table_name = std::string(update_log_record->table_name_, update_log_record->table_name_size_);
        } else if(log_record->log_type_ == LogType::INSERT)
        {
            std::shared_ptr<InsertLogRecord> insert_log_record = std::dynamic_pointer_cast<InsertLogRecord>(log_record);
            page_id = insert_log_record->rid_.page_no;
            table_name = std::string(insert_log_record->table_name_, insert_log_record->table_name_size_);
        } else if(log_record->log_type_ == LogType::DELETE)
        {
            std::shared_ptr<DeleteLogRecord> delete_log_record = std::dynamic_pointer_cast<DeleteLogRecord>(log_record);
            page_id = delete_log_record->rid_.page_no;
            table_name = std::string(delete_log_record->table_name_, delete_log_record->table_name_size_);
        }
        auto fh = sm_manager_->fhs_[table_name].get();
        if(fh == nullptr)
        {
            throw TableNotFoundError(table_name);
        }
        PageId pid = {fh->GetFd(), page_id};
        if(redo_logs_map_.find(pid) == redo_logs_map_.end())
        {
            redo_logs_map_.emplace(pid, RedoLogsInPage());
            redo_logs_map_[pid].table_file_ = fh;
        }
        redo_logs_map_[pid].redo_logs_.push_back(it->first);
    }
}

/**
 * @description: 重做所有未落盘的操作
 */
void RecoveryManager::redo() {

    for(auto& redo_log : redo_logs_map_)
    {
        auto& redo_logs = redo_log.second.redo_logs_;
        auto& table_file = redo_log.second.table_file_;
        for(auto& lsn : redo_logs)
        {
            auto& log_record = log_records_[lsn];
            if(log_record->log_type_ == LogType::UPDATE)
            {
                std::shared_ptr<UpdateLogRecord> update_log_record = std::dynamic_pointer_cast<UpdateLogRecord>(log_record);
                table_file->update_record(update_log_record->rid_, update_log_record->update_value_.data, nullptr);
            } else if(log_record->log_type_ == LogType::INSERT)
            {
                std::shared_ptr<InsertLogRecord> insert_log_record = std::dynamic_pointer_cast<InsertLogRecord>(log_record);
                table_file->insert_record(insert_log_record->rid_, insert_log_record->insert_value_.data);
            } else if(log_record->log_type_ == LogType::DELETE)
            {
                std::shared_ptr<DeleteLogRecord> delete_log_record = std::dynamic_pointer_cast<DeleteLogRecord>(log_record);
                table_file->delete_record(delete_log_record->rid_, nullptr);
            }
        }
    }
}

/**
 * @description: 回滚未完成的事务
 */
void RecoveryManager::undo() {
    std::vector<txn_id_t> undo_txn_list;
    for(auto& txn_id : txn_list_)
    {
        if(undo_txn_.find(txn_id) != undo_txn_.end())
        {
            undo_txn_list.push_back(txn_id);
        }
    }
    for(auto it = undo_txn_list.rbegin(); it != undo_txn_list.rend(); it++)
    {
        txn_id_t txn_id = *it;
        lsn_t curr_lsn = undo_txn_[txn_id];
        while(txn_id != INVALID_LSN)
        {
            auto& log_record = log_records_[curr_lsn];
            if(!log_record)
                break;
            if(log_record->log_type_ == LogType::UPDATE)
            {
                std::shared_ptr<UpdateLogRecord> update_log_record = std::dynamic_pointer_cast<UpdateLogRecord>(log_record);
                RmFileHandle* table_file = sm_manager_->fhs_[std::string(update_log_record->table_name_, update_log_record->table_name_size_)].get();
                table_file->update_record(update_log_record->rid_, update_log_record->before_value_.data, nullptr);
            } else if(log_record->log_type_ == LogType::INSERT)
            {
                std::shared_ptr<InsertLogRecord> insert_log_record = std::dynamic_pointer_cast<InsertLogRecord>(log_record);
                RmFileHandle* table_file = sm_manager_->fhs_[std::string(insert_log_record->table_name_, insert_log_record->table_name_size_)].get();
                table_file->delete_record(insert_log_record->rid_, nullptr);
            } else if(log_record->log_type_ == LogType::DELETE)
            {
                std::shared_ptr<DeleteLogRecord> delete_log_record = std::dynamic_pointer_cast<DeleteLogRecord>(log_record);
                RmFileHandle* table_file = sm_manager_->fhs_[std::string(delete_log_record->table_name_, delete_log_record->table_name_size_)].get();
                table_file->insert_record(delete_log_record->rid_, delete_log_record->delete_value_.data);
            }
            curr_lsn = log_record->prev_lsn_;
        }
        
    }
    // 删除索引，重新建立索引
    if(redo_txn_.size() == 0 && undo_txn_.size() == 0)
    {
        return;
    }
    for(auto& table : sm_manager_->fhs_)
    {
        auto& tab = sm_manager_->db_.get_table(table.first);
        std::vector<IndexMeta> indexes = tab.indexes;
        for(auto& index : indexes)
        {
            std::vector<std::string> index_cols;
            for(auto& col : index.cols)
            {
                index_cols.push_back(col.name);
            }
            sm_manager_->drop_index(table.first, index.cols, nullptr);
            sm_manager_->create_index(table.first, index_cols, nullptr);
        }
    }
}