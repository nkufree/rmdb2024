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

#include "common/config.h"

/**
 * Replacer 是一个抽象类，用于跟踪页面使用情况。
 */
class Replacer
{
public:
        Replacer() = default;
        virtual ~Replacer() = default;

        /**
         * 根据替换策略移除 victim 帧。
         * @param[out] frame_id 被移除的帧的 ID，如果没有找到 victim 则为 nullptr
         * @return 如果找到 victim 帧返回 true，否则返回 false
         */
        virtual bool victim(frame_id_t *frame_id) = 0;

        /**
         * 固定一个帧，表示在其被取消固定之前不应该被选为 victim
         * @param frame_id 要固定的帧的 ID
         */
        virtual void pin(frame_id_t frame_id) = 0;

        /**
         * 取消固定一个帧，表示它现在可以被选为 victim
         * @param frame_id 要取消固定的帧的 ID
         */
        virtual void unpin(frame_id_t frame_id) = 0;

        /** @return 返回 replacer 中可以被选为 victim 的元素数量 */
        virtual size_t Size() = 0;
};
