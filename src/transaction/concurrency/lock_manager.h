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

#include <mutex>
#include <condition_variable>
#include "transaction/transaction.h"

static const std::string GroupLockModeStr[10] = {"NON_LOCK", "IS", "IX", "S", "X", "SIX"};

class LockManager {
    /* 加锁类型，包括共享锁、排他锁、意向共享锁、意向排他锁、SIX（意向排他锁+共享锁） */
    enum class LockMode { SHARED, EXLUCSIVE, INTENTION_SHARED, INTENTION_EXCLUSIVE, S_IX };

    /* 用于标识加锁队列中排他性最强的锁类型，例如加锁队列中有SHARED和EXLUSIVE两个加锁操作，则该队列的锁模式为X */
    enum class GroupLockMode { NON_LOCK, IS, IX, S, X, SIX};

    bool lock_matrix_[6][6] = {
                        /* NON_LOCK, IS, IX, S, X, SIX */
        /* NON_LOCK */  {true, true, true, true, true, true},
        /* IS */        {true, true, true, true, false, true},
        /* IX */        {true, true, true, false, false, false},
        /* S */         {true, true, false, true, false, false},
        /* X */         {true, false, false, false, false, false},
        /* SIX */       {true, true, false, false, false, false}
    };


    /* 事务的加锁申请 */
    class LockRequest {
    public:
        LockRequest(txn_id_t txn_id, LockMode lock_mode)
            : txn_id_(txn_id), lock_mode_(lock_mode), granted_(false) {}

        txn_id_t txn_id_;   // 申请加锁的事务ID
        LockMode lock_mode_;    // 事务申请加锁的类型
        bool granted_;          // 该事务是否已经被赋予锁
    };

    /* 数据项上的加锁队列 */
    class LockRequestQueue {
    public:
        std::list<std::shared_ptr<LockRequest>> request_queue_;  // 加锁队列
        std::list<std::shared_ptr<LockRequest>> upgrade_queue_;  // 升级队列
        std::mutex latch_;     // 用于加锁队列的互斥锁
        std::condition_variable cv_;            // 条件变量，用于唤醒正在等待加锁的申请，在no-wait策略下无需使用
        GroupLockMode group_lock_mode_ = GroupLockMode::NON_LOCK;   // 加锁队列的锁模式，即队列中已获取锁的最高级别
        LockRequestQueue() = default;
    };

public:
    LockManager() {}

    ~LockManager() {}

    bool lock_shared_on_record(Transaction* txn, const Rid& rid, int tab_fd);

    bool lock_exclusive_on_record(Transaction* txn, const Rid& rid, int tab_fd);

    bool lock_shared_on_table(Transaction* txn, int tab_fd);

    bool lock_exclusive_on_table(Transaction* txn, int tab_fd);

    bool lock_IS_on_table(Transaction* txn, int tab_fd);

    bool lock_IX_on_table(Transaction* txn, int tab_fd);

    bool unlock(Transaction* txn, LockDataId lock_data_id);


private:
    inline void check_wait_die(const std::shared_ptr<LockRequestQueue>& lock_request_queue, Transaction* txn);
    inline std::shared_ptr<LockRequestQueue> get_lock_request_queue(const LockDataId& lock_data_id);
    bool check_and_execute_lock(std::shared_ptr<LockRequestQueue> lock_request_queue, std::shared_ptr<LockRequest> lock_request, Transaction* txn, GroupLockMode lock_mode);

    bool upgrade_lock_on_table(Transaction* txn, int tab_fd, LockMode lock_mode);

    bool upgrade_lock_on_record(Transaction* txn, const Rid& rid, int tab_fd);

    inline bool lock_compatible(GroupLockMode a, GroupLockMode b) {
        return lock_matrix_[static_cast<int>(a)][static_cast<int>(b)];
    }

    GroupLockMode get_group_lock_mode(LockMode lock_mode);

    std::mutex latch_;      // 用于锁表的并发
    std::unordered_map<LockDataId, std::shared_ptr<LockRequestQueue>> lock_table_;   // 全局锁表
};
