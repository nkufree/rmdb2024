/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "transaction_manager.h"
#include "record/rm_file_handle.h"
#include "system/sm_manager.h"

std::unordered_map<txn_id_t, Transaction *> TransactionManager::txn_map = {};

/**
 * @description: 事务的开始方法
 * @return {Transaction*} 开始事务的指针
 * @param {Transaction*} txn 事务指针，空指针代表需要创建新事务，否则开始已有事务
 * @param {LogManager*} log_manager 日志管理器指针
 */
Transaction * TransactionManager::begin(Transaction* txn, LogManager* log_manager) {
    // Todo:
    // 1. 判断传入事务参数是否为空指针
    // 2. 如果为空指针，创建新事务
    // 3. 把开始事务加入到全局事务表中
    // 4. 返回当前事务指针
    Transaction* res;
    if(txn != nullptr) {
        return txn;
    }
    std::unique_lock<std::shared_mutex> lock(latch_);
    res = new Transaction(next_txn_id_);
    txn_map[next_txn_id_] = res;
    res->set_start_ts(next_timestamp_);
    next_txn_id_++;
    next_timestamp_++;
    BeginLogRecord log_record(res->get_transaction_id());
    log_record.prev_lsn_ = res->get_prev_lsn();
    lsn_t curr_lsn = log_manager->add_log_to_buffer(&log_record);
    res->set_prev_lsn(curr_lsn);
    res->set_state(TransactionState::DEFAULT);
    return res;
}

/**
 * @description: 事务的提交方法
 * @param {Transaction*} txn 需要提交的事务
 * @param {LogManager*} log_manager 日志管理器指针
 */
void TransactionManager::commit(Transaction* txn, LogManager* log_manager) {
    // Todo:
    // 1. 如果存在未提交的写操作，提交所有的写操作
    // 2. 释放所有锁
    // 3. 释放事务相关资源，eg.锁集
    // 4. 把事务日志刷入磁盘中
    // 5. 更新事务状态
    CommitLogRecord log_record(txn->get_transaction_id());
    log_record.prev_lsn_ = txn->get_prev_lsn();
    lsn_t curr_lsn = log_manager->add_log_to_buffer(&log_record);
    txn->set_prev_lsn(curr_lsn);
    for(auto it = txn->get_write_set()->begin(); it != txn->get_write_set()->end();)
    {
        delete *it;
        it = txn->get_write_set()->erase(it);
    }
    // for(auto it = txn->get_lock_set()->begin(); it != txn->get_lock_set()->end();)
    // {
    //     if(it->type_ == LockDataType::RECORD)
    //     {
    //         lock_manager_->unlock(txn, *it);
    //         it = txn->get_lock_set()->erase(it);
    //     }
    //     else
    //         it++;
    // }
    for(auto it = txn->get_lock_set()->begin(); it != txn->get_lock_set()->end();)
    {
        lock_manager_->unlock(txn, *it);
        it = txn->get_lock_set()->erase(it);
    }
    log_manager->flush_log_to_disk();
    txn->set_state(TransactionState::COMMITTED);
    cv_.notify_all();
}

/**
 * @description: 事务的终止（回滚）方法
 * @param {Transaction *} txn 需要回滚的事务
 * @param {LogManager} *log_manager 日志管理器指针
 */
void TransactionManager::abort(Transaction * txn, LogManager *log_manager) {
    // Todo:
    // 1. 回滚所有写操作
    // 2. 释放所有锁
    // 3. 清空事务相关资源，eg.锁集
    // 4. 把事务日志刷入磁盘中
    // 5. 更新事务状态
    AbortLogRecord log_record(txn->get_transaction_id());
    log_record.prev_lsn_ = txn->get_prev_lsn();
    lsn_t curr_lsn = log_manager->add_log_to_buffer(&log_record);
    txn->set_prev_lsn(curr_lsn);
    Context context(lock_manager_, log_manager, txn);
    auto write_set = txn->get_write_set();
    while (!write_set->empty())
    {
        WriteRecord *write_record = write_set->back();
        write_set->pop_back();
        std::string& tab_name = write_record->GetTableName();
        TabMeta& tab = sm_manager_->db_.get_table(tab_name);
        RmFileHandle *fh_ = sm_manager_->fhs_.at(tab_name).get();
        WType type = write_record->GetWriteType();
        if(type == WType::DELETE_TUPLE)
        {
            fh_->insert_record(write_record->GetRid(), write_record->GetRecord().data);
        }
        // 修改索引
        for (auto &index : tab.indexes)
        {
            auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols)).get();
            char *key = new char[tab.get_col_total_len()];
            int offset = 0;
            char* record = write_record->GetRecord().data;
            std::unique_ptr<RmRecord> rec;
            if(type == WType::INSERT_TUPLE || type == WType::UPDATE_TUPLE) {
                rec = fh_->get_record(write_record->GetRid(), &context);
                record = rec->data;
            }
            for (size_t j = 0; j < (size_t)index.col_num; ++j)
            {
                memcpy(key + offset, record + index.cols[j].offset, index.cols[j].len);
                offset += index.cols[j].len;
            }
            char* old_rec;
            char* old_key = new char[tab.get_col_total_len()];
            switch (type)
            {
            case WType::INSERT_TUPLE:
                ih->delete_entry(key, txn, &write_record->GetRid());
                break;
            case WType::DELETE_TUPLE:
                ih->insert_entry(key, write_record->GetRid(), txn);
                break;
            case WType::UPDATE_TUPLE:
                ih->delete_entry(key, txn, &write_record->GetRid());
                old_rec = write_record->GetRecord().data;
                offset = 0;
                for (size_t j = 0; j < (size_t)index.col_num; ++j)
                {
                    memcpy(old_key + offset, old_rec + index.cols[j].offset, index.cols[j].len);
                    offset += index.cols[j].len;
                }
                ih->insert_entry(old_key, write_record->GetRid(), txn);
                break;
            default:
                break;
            }
            delete old_key;
            delete[] key;
        }
        // 修改表中的数据
        switch (type)
        {
        case WType::INSERT_TUPLE:
            fh_->delete_record(write_record->GetRid(), nullptr);
            break;
        case WType::UPDATE_TUPLE:
            fh_->update_record(write_record->GetRid(), write_record->GetRecord().data, nullptr);
            break;
        default:
            break;
        }
        delete write_record;
    }
    // 释放锁
    // for(auto it = txn->get_lock_set()->begin(); it != txn->get_lock_set()->end();)
    // {
    //     if(it->type_ == LockDataType::RECORD)
    //     {
    //         lock_manager_->unlock(txn, *it);
    //         it = txn->get_lock_set()->erase(it);
    //     }
    //     else
    //         it++;
    // }
    for(auto it = txn->get_lock_set()->begin(); it != txn->get_lock_set()->end();)
    {
        lock_manager_->unlock(txn, *it);
        it = txn->get_lock_set()->erase(it);
    }
    log_manager->flush_log_to_disk();
    txn->set_state(TransactionState::ABORTED);
    for(auto it = txn->get_write_set()->begin(); it != txn->get_write_set()->end();)
    {
        delete *it;
        it = txn->get_write_set()->erase(it);
    }
    cv_.notify_all();
}

void TransactionManager::create_static_checkpoint(Context* context, SmManager* sm_manager)
{
    // 停止接受新事务
    std::shared_lock<std::shared_mutex> lock(latch_);
    this->commit(context->txn_, context->log_mgr_);
    // 等待现有事务完成
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);
    cv_.wait(lck, [&]() {
        return std::all_of(txn_map.begin(), txn_map.end(), [&](const auto& pair) {
            return pair.second->get_state() == TransactionState::COMMITTED || pair.second->get_state() == TransactionState::ABORTED;
        });
    });
    // 刷新磁盘
    sm_manager->flush_all_pages();
    // 添加日志
    StaticCKPTLogRecord log_record;
    context->log_mgr_->add_static_CKPT(&log_record);

}
