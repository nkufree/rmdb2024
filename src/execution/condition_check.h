#pragma once
#include "common/common.h"
#include "system/sm_meta.h"

class ConditionCheck{
public:
    static bool check_conditions(const std::vector<Condition>& conds, const std::vector<ColMeta>& cols, const std::unique_ptr<RmRecord>& rec)
    {
        for(auto& cond : conds)
        {
            if(!check_single_condition(cond, cols, rec))
                return false;
        }
        return true;
    }

    static bool check_single_condition(const Condition& cond, const std::vector<ColMeta>& cols, const std::unique_ptr<RmRecord>& rec)
    {
        // 1. 构建两个比较的value
        // const std::string& lhs_col_name = cond.lhs_col.col_name;
        // ColType lhs_type;
        // int lhs_offset;
        // for(auto& col : cols)
        // {
        //     if(strcmp(col.name.c_str(), lhs_col_name.c_str()) == 0)
        //     {
        //         lhs_type = col.type;
        //         lhs_offset = col.offset;
        //         break;
        //     }
        // }
        auto lhs_match_col = AbstractExecutor::get_col(cols, cond.lhs_col);
        Value lhs_val = build_value(lhs_match_col->type, lhs_match_col->offset, rec);
        Value rhs_val = cond.rhs_val;
        if(!cond.is_rhs_val)
        {
            // ColType rhs_type;
            // int rhs_offset;
            // for(auto& col : cols)
            // {
            //     if(strcmp(col.name.c_str(), rhs_col_name.c_str()) == 0)
            //     {
            //         rhs_type = col.type;
            //         rhs_offset = col.offset;
            //         break;
            //     }
            // }
            auto rhs_match_col = AbstractExecutor::get_col(cols, cond.rhs_col);
            rhs_val = build_value(rhs_match_col->type, rhs_match_col->offset, rec);
        }
        // 2. 类型转换
        if(lhs_val.type != rhs_val.type)
        {
            if(lhs_val.type == ColType::TYPE_INT && rhs_val.type == ColType::TYPE_FLOAT)
            {
                // lhs_val.set_float(lhs_val.int_val);
                rhs_val.set_int(rhs_val.float_val);
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
        return compare_value(lhs_val, rhs_val, cond.op);
    }

    // 将val2的类型转换为val1的类型
    static void value_transfer(Value& val, ColType type)
    {
        if(val.type == type)
            return;
        if(val.type == ColType::TYPE_INT && type == ColType::TYPE_FLOAT)
        {
            val.set_float(val.int_val);
        }
        else if(val.type == ColType::TYPE_FLOAT && type == ColType::TYPE_INT)
        {
            val.set_int(val.float_val);
        }
        else
        {
            throw IncompatibleTypeError(coltype2str(val.type), coltype2str(type));
        }
    }
private:
    static Value build_value(ColType col_type, int offset, const std::unique_ptr<RmRecord>& rec)
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
            break;
        default:
            break;
        }
        return val;
    }

    static bool value_equal(const Value& lhs, const Value& rhs)
    {
        if(lhs.type != rhs.type)
            return false;
        switch (lhs.type)
        {
        case ColType::TYPE_INT:
            return lhs.int_val == rhs.int_val;
            break;
        case ColType::TYPE_FLOAT:
            return lhs.float_val == rhs.float_val;
            break;
        case ColType::TYPE_STRING:
            return lhs.str_val == rhs.str_val;
            break;
        default:
            break;
        }
        throw InternalError("Unexpected value type");
    }

    static bool value_less(const Value& lhs, const Value& rhs)
    {
        if(lhs.type != rhs.type)
            return false;
        switch (lhs.type)
        {
        case ColType::TYPE_INT:
            return lhs.int_val < rhs.int_val;
            break;
        case ColType::TYPE_FLOAT:
            return lhs.float_val < rhs.float_val;
            break;
        case ColType::TYPE_STRING:
            return lhs.str_val < rhs.str_val;
            break;
        default:
            break;
        }
        throw InternalError("Unexpected value type");
    }

    static bool value_greater(const Value& lhs, const Value& rhs)
    {
        if(lhs.type != rhs.type)
            return false;
        switch (lhs.type)
        {
        case ColType::TYPE_INT:
            return lhs.int_val > rhs.int_val;
            break;
        case ColType::TYPE_FLOAT:
            return lhs.float_val > rhs.float_val;
            break;
        case ColType::TYPE_STRING:
            return lhs.str_val > rhs.str_val;
            break;
        default:
            break;
        }
        throw InternalError("Unexpected value type");
    }

    static bool compare_value(const Value& lhs, const Value& rhs, CompOp op)
    {
        switch (op)
        {
        case CompOp::OP_EQ:
            return value_equal(lhs, rhs);
            break;
        case CompOp::OP_NE:
            return !value_equal(lhs, rhs);
            break;
        case CompOp::OP_LT:
            return value_less(lhs, rhs);
            break;
        case CompOp::OP_GT:
            return value_greater(lhs, rhs);
            break;
        case CompOp::OP_LE:
            return !value_greater(lhs, rhs);
            break;
        case CompOp::OP_GE:
            return !value_less(lhs, rhs);
            break;
        default:
            break;
        }
        throw InternalError("Unexpected comparison operator");
    }
};
