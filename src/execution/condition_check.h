#pragma once
#include "common/common.h"
#include "system/sm_meta.h"

class ConditionCheck{
public:
    static bool check_conditions(const std::vector<Condition>& conds, const std::vector<ColMeta>& cols, const std::unique_ptr<RmRecord>& rec)
    {
        bool res = std::all_of(conds.begin(), conds.end(), [&](const Condition& cond) { return check_single_condition(cond, cols, rec); });
        return res;
    }

    static bool check_single_condition(const Condition& cond, const std::vector<ColMeta>& cols, const std::unique_ptr<RmRecord>& rec)
    {
        auto lhs_match_col = AbstractExecutor::get_col(cols, cond.lhs_col);
        Value lhs_val = build_value(lhs_match_col->type, lhs_match_col->offset, rec, lhs_match_col->len);
        Value rhs_val = cond.rhs_val;
        if(!cond.is_rhs_val)
        {
            auto rhs_match_col = AbstractExecutor::get_col(cols, cond.rhs_col);
            rhs_val = build_value(rhs_match_col->type, rhs_match_col->offset, rec, rhs_match_col->len);
        }
        // 2. 类型转换
        if(lhs_val.type != rhs_val.type)
        {
            if(lhs_val.type == ColType::TYPE_INT && rhs_val.type == ColType::TYPE_FLOAT)
            {
                lhs_val.set_float(lhs_val.int_val);
                // rhs_val.set_int(rhs_val.float_val);
            }
            else if(lhs_val.type == ColType::TYPE_FLOAT && rhs_val.type == ColType::TYPE_INT)
            {
                rhs_val.set_float(rhs_val.int_val);
            }
            else
            {
                throw IncompatibleTypeError(coltype2str(lhs_val.type), coltype2str(rhs_val.type));
            }
        }
        // 3. 比较
        return cond.check_condition(lhs_val, rhs_val);
    }
private:
    static Value build_value(ColType col_type, int offset, const std::unique_ptr<RmRecord>& rec, int len)
    {
        Value val;
        switch (col_type)
        {
        case ColType::TYPE_INT:
            val.set_int(*(int*)(uintptr_t)(rec->data + offset));
            break;
        case ColType::TYPE_FLOAT:
            val.set_float(*(float*)(uintptr_t)(rec->data + offset));
            break;
        case ColType::TYPE_STRING:
            val.set_str(std::string((char*)(uintptr_t)(rec->data + offset)));
            val.str_len = len;
            break;
        default:
            break;
        }
        return val;
    }
};
