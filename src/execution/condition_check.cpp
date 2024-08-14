#include "condition_check.h"
#include "portal.h"
#include "execution/executor_abstract.h"

void ConditionCheck::execute_sub_query(Condition& cond)
{
    if(!cond.is_rhs_select) return;
    std::shared_ptr<PortalStmt> portal = cond.rhs_portal;
    std::unique_ptr<AbstractExecutor> executeRoot = std::move(portal->root);
    if(cond.op != CompOp::OP_IN)
    {
        executeRoot->beginTuple();
        auto Tuple = executeRoot->Next();
        executeRoot->nextTuple();
        if(!executeRoot->is_end())
        {
            throw InternalError("Subquery returns more than one row");
        }
        const std::vector<ColMeta>& cols = executeRoot->cols();
        assert(cols.size() == 1);
        const ColMeta& col = cols[0];
        char *rec_buf = Tuple->data;
        switch (col.type)
        {
        case ColType::TYPE_INT:
            cond.rhs_val.set_int(*(int *)rec_buf);
            break;
        case ColType::TYPE_FLOAT:
            cond.rhs_val.set_float(*(float *)rec_buf);
            break;
        case ColType::TYPE_STRING:
            cond.rhs_val.set_str(std::string((char *)rec_buf, col.len));
            break;
        default:
            break;
        }
        cond.is_rhs_select = false;
        cond.is_rhs_val = true;
        cond.rhs_type = CondRhsType::RHS_VALUE;
    }
    else
    {
        for (executeRoot->beginTuple(); !executeRoot->is_end(); executeRoot->nextTuple()) {
            auto Tuple = executeRoot->Next();
            const std::vector<ColMeta>& cols = executeRoot->cols();
            assert(cols.size() == 1);
            const ColMeta& col = cols[0];
            char *rec_buf = Tuple->data;
            Value val;
            switch (col.type)
            {
            case ColType::TYPE_INT:
                val.set_int(*(int *)rec_buf);
                break;
            case ColType::TYPE_FLOAT:
                val.set_float(*(float *)rec_buf);
                break;
            case ColType::TYPE_STRING:
                val.set_str(std::string((char *)rec_buf, col.len));
                break;
            default:
                break;
            }
            cond.rhs_set.insert(val);
        }
        cond.rhs_type = CondRhsType::RHS_SET;
    }
}

bool ConditionCheck::check_conditions(std::vector<Condition>& conds, const std::unique_ptr<RmRecord>& rec)
{
    bool res = std::all_of(conds.begin(), conds.end(), [&](Condition& cond) { return check_single_condition(cond, rec); });
    return res;
}

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

bool ConditionCheck::check_single_condition(Condition& cond, const std::unique_ptr<RmRecord>& rec)
{
    auto& lhs_match_col = cond.lhs_match_col;
    Value lhs_val = build_value(lhs_match_col->type, lhs_match_col->offset, rec, lhs_match_col->len);
    Value rhs_val = cond.rhs_val;
    if(cond.rhs_type == CondRhsType::RHS_COL)
    {
        auto& rhs_match_col = cond.rhs_match_col;
        rhs_val = build_value(rhs_match_col->type, rhs_match_col->offset, rec, rhs_match_col->len);
    }
    // 2. 类型转换
    if(cond.rhs_type == CondRhsType::RHS_VALUE && lhs_val.type != rhs_val.type)
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
    else if(cond.rhs_type == CondRhsType::RHS_SET && cond.rhs_set.size() > 0 && lhs_val.type != cond.rhs_set.begin()->type)
    {
        std::set<Value> new_set;
        for(auto &val : cond.rhs_set)
        {
            Value new_val = val;
            new_val.value_cast(lhs_val.type);
            new_set.insert(new_val);
        }
        cond.rhs_set.swap(new_set);
    }
    // 3. 比较
    return cond.check_condition(lhs_val, rhs_val);
    }