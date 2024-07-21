/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "lock_manager.h"
#include <algorithm>

/**
 * @description: 申请行级共享锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID 记录所在的表的fd
 * @param {int} tab_fd
 */
bool LockManager::lock_shared_on_record(Transaction* txn, const Rid& rid, int tab_fd) {
    
    return true;
}

/**
 * @description: 申请行级排他锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID
 * @param {int} tab_fd 记录所在的表的fd
 */
bool LockManager::lock_exclusive_on_record(Transaction* txn, const Rid& rid, int tab_fd) {

    return true;
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {
    
    std::cout << "txn " << txn->get_transaction_id() << " lock_shared_on_table " << tab_fd << std::endl;
    const LockDataId lock_data_id = LockDataId(tab_fd, LockDataType::TABLE);
    std::shared_ptr<LockRequestQueue> lock_request_queue;
    {
        std::scoped_lock<std::mutex> lock(latch_);
        if(lock_table_.find(lock_data_id) == lock_table_.end()) {
            std::shared_ptr<LockRequestQueue> lock_request_queue = std::make_shared<LockRequestQueue>();
            lock_table_.emplace(lock_data_id, lock_request_queue);
        }
        lock_request_queue = lock_table_[lock_data_id];
    }
    std::shared_ptr<LockRequest> lock_request = std::make_shared<LockRequest>(txn->get_transaction_id(), LockMode::SHARED);
    std::unique_lock<std::mutex> queue_lock(lock_request_queue->latch_);
    // 检查是否有事务已经持有排他锁
    // 如果有事务持有排他锁，加入等待队列，等待条件变量唤醒
    if(lock_request_queue->group_lock_mode_ == GroupLockMode::X) {
        bool wait_die = std::any_of(lock_request_queue->request_queue_.begin(), lock_request_queue->request_queue_.end(), [&](const std::shared_ptr<LockRequest>& lock_request){
            return lock_request->txn_id_ < txn->get_transaction_id();
        });
        if(wait_die)
        {
            throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
        }
        lock_request_queue->request_queue_.emplace_back(lock_request);
        lock_request_queue->cv_.wait(queue_lock, [&](){
            return lock_request->granted_;
        });
    }
    else
    {
        lock_request->granted_ = true;
        lock_request_queue->request_queue_.emplace_back(lock_request);
        lock_request_queue->group_lock_mode_ = GroupLockMode::S;
    }
    txn->get_lock_set()->insert(lock_data_id);
    return true;
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {
    std::cout << "txn " << txn->get_transaction_id() << " lock_exclusive_on_table " << tab_fd << std::endl;
    const LockDataId lock_data_id = LockDataId(tab_fd, LockDataType::TABLE);
    std::shared_ptr<LockRequestQueue> lock_request_queue;
    {
        std::scoped_lock<std::mutex> lock(latch_);
        if(lock_table_.find(lock_data_id) == lock_table_.end()) {
            std::shared_ptr<LockRequestQueue> lock_request_queue = std::make_shared<LockRequestQueue>();
            lock_table_.emplace(lock_data_id, lock_request_queue);
        }
        lock_request_queue = lock_table_[lock_data_id];
    }
    std::shared_ptr<LockRequest> lock_request = std::make_shared<LockRequest>(txn->get_transaction_id(), LockMode::EXLUCSIVE);
    // 检查是否有事务已经持有锁
    std::unique_lock<std::mutex> queue_lock(lock_request_queue->latch_);
    if(lock_request_queue->group_lock_mode_ != GroupLockMode::NON_LOCK) {
        // wait-die
        bool wait_die = std::any_of(lock_request_queue->request_queue_.begin(), lock_request_queue->request_queue_.end(), [&](const std::shared_ptr<LockRequest>& lock_request){
            return lock_request->txn_id_ < txn->get_transaction_id();
        });
        if(wait_die)
        {
            throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
        }
        lock_request_queue->request_queue_.emplace_back(lock_request);
        lock_request_queue->cv_.wait(queue_lock, [&](){
            return lock_request->granted_;
        });
    }
    else
    {
        lock_request->granted_ = true;
        lock_request_queue->request_queue_.emplace_back(lock_request);
        lock_request_queue->group_lock_mode_ = GroupLockMode::X;
    }
    txn->get_lock_set()->insert(lock_data_id);
    return true;
}

/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) {
    
    return true;
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {
    
    return true;
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {
    std::shared_ptr<LockRequestQueue> lock_request_queue;
    {
        std::scoped_lock<std::mutex> lock(latch_);
        if(lock_table_.find(lock_data_id) == lock_table_.end()) {
            return false;
        }
        lock_request_queue = lock_table_[lock_data_id];
    }
    std::unique_lock<std::mutex> queue_lock(lock_request_queue->latch_);
    auto& request_queue = lock_request_queue->request_queue_;
    for(auto it = request_queue.begin(); it != request_queue.end(); it++) {
        if((*it)->txn_id_ == txn->get_transaction_id()) {
            request_queue.erase(it);
            break;
        }
    }
    if(request_queue.empty()) {
        lock_request_queue->group_lock_mode_ = GroupLockMode::NON_LOCK;
    }
    else if(!std::any_of(request_queue.begin(), request_queue.end(), [&](const std::shared_ptr<LockRequest>& lock_request){
        return lock_request->granted_;
    }))
    {
        // 更新加锁队列的锁模式
        lock_request_queue->group_lock_mode_ = request_queue.front()->lock_mode_ == LockMode::SHARED ? GroupLockMode::S : GroupLockMode::X;
        // 按照加锁队列唤醒等待的事务
        if(lock_request_queue->group_lock_mode_ == GroupLockMode::S) {
            for(auto& lock_request : request_queue) {
                if(lock_request->lock_mode_ == LockMode::SHARED) {
                    lock_request->granted_ = true;
                }
                else
                {
                    continue;
                }
            }
        }
        else if(lock_request_queue->group_lock_mode_ == GroupLockMode::X)
        {
            request_queue.front()->granted_ = true;
        }
        lock_request_queue->cv_.notify_all();
    }
    return true;
}