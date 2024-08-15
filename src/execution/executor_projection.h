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

class ProjectionExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_;        // 投影节点的儿子节点
    std::vector<ColMeta> cols_;                     // 需要投影的字段
    size_t len_;                                    // 字段总长度
    std::vector<size_t> sel_idxs_;                  
    bool is_sel_all_ = true;                                // 是否选择全部字段

   public:
    ProjectionExecutor(std::unique_ptr<AbstractExecutor> prev, const std::vector<TabCol> &sel_cols) {
        prev_ = std::move(prev);

        size_t curr_offset = 0;
        auto &prev_cols = prev_->cols();
        for (auto &sel_col : sel_cols) {
            auto pos = get_col(prev_cols, sel_col, true);
            sel_idxs_.push_back(pos - prev_cols.begin());
            auto col = *pos;
            col.offset = curr_offset;
            curr_offset += col.len;
            cols_.push_back(col);
        }
        len_ = curr_offset;
        if(sel_cols.size() != prev_cols.size())
        {
            is_sel_all_ = false;
        }
        else
        {
            for(size_t i = 0; i < sel_idxs_.size(); i++)
            {
                if(sel_idxs_[i] != i)
                {
                    is_sel_all_ = false;
                    break;
                }
            }
        }
    }

    void beginTuple() override { prev_->beginTuple(); }

    void nextTuple() override { prev_->nextTuple(); }

    std::unique_ptr<RmRecord> Next() override {
        // TODO(zzx): 可以优化，如果投影的是表中的全部字段则直接返回
        auto rec = prev_->Next();
        if(is_sel_all_)
        {
            return rec;
        }
        auto &prev_cols = prev_->cols();
        std::unique_ptr<RmRecord> new_rec = std::make_unique<RmRecord>(len_);
        int curr_offset = 0;
        for (size_t i = 0; i < cols_.size(); i++){
            size_t col_offset = prev_cols[sel_idxs_[i]].offset;
            memcpy(new_rec->data + curr_offset, rec->data + col_offset, cols_[i].len);
            curr_offset += cols_[i].len;
        }
        return new_rec;
    }

    bool is_end() const override { return prev_->is_end(); }

    const std::vector<ColMeta> &cols() const override { return cols_; }

    size_t tupleLen() const override { return len_; }

    Rid &rid() override { return _abstract_rid; }

    int get_count() {
        return prev_->get_count();
    }
};