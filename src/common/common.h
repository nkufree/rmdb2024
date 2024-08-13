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

#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include "defs.h"
#include "record/rm_defs.h"
#include "parser/ast.h"
#include <set>


struct TabCol {
    std::string tab_name;
    std::string col_name;
    std::string alias;
    ast::AggregationType aggr; // see AggregationType in ast.h

    friend bool operator<(const TabCol &x, const TabCol &y) {
        return std::make_pair(x.tab_name, x.col_name) < std::make_pair(y.tab_name, y.col_name);
    }
};

struct Value {
    ColType type;  // type of value
    union {
        int int_val;      // int value
        float float_val;  // float value
    };
    std::string str_val;  // string value

    std::shared_ptr<RmRecord> raw;  // raw record buffer
    int str_len;  // length of string value
    void set_int(int int_val_) {
        type = TYPE_INT;
        int_val = int_val_;
    }

    void set_float(float float_val_) {
        type = TYPE_FLOAT;
        float_val = float_val_;
    }

    void set_str(std::string str_val_) {
        type = TYPE_STRING;
        str_val = std::move(str_val_);
    }

    void init_raw(int len) {
        assert(raw == nullptr);
        raw = std::make_shared<RmRecord>(len);
        if (type == TYPE_INT) {
            assert(len == sizeof(int));
            *(int *)(raw->data) = int_val;
        } else if (type == TYPE_FLOAT) {
            assert(len == sizeof(float));
            *(float *)(raw->data) = float_val;
        } else if (type == TYPE_STRING) {
            if (len < (int)str_val.size()) {
                throw StringOverflowError();
            }
            memcpy(raw->data, str_val.c_str(), str_val.size());
            memset(raw->data + str_val.size(), 0, len - str_val.size());
        }
    }

    void update_raw() {
        if (type == TYPE_INT) {
            *(int *)(raw->data) = int_val;
        } else if (type == TYPE_FLOAT) {
            *(float *)(raw->data) = float_val;
        } else if (type == TYPE_STRING) {
            if (str_len < (int)str_val.size()) {
                throw StringOverflowError();
            }
            memset(raw->data, 0, str_len);
            memcpy(raw->data, str_val.c_str(), str_val.size());
        }
    }

    void value_cast(ColType new_type) {
        if (type == new_type) return;
        if (type == TYPE_INT && new_type == TYPE_FLOAT) {
            float_val = int_val;
        } else if (type == TYPE_FLOAT && new_type == TYPE_INT) {
            int_val = float_val;
        } else if(type == TYPE_STRING && new_type == TYPE_INT) {
            sscanf(str_val.c_str(), "%d", &int_val);
        } else if(type == TYPE_STRING && new_type == TYPE_FLOAT) {
            sscanf(str_val.c_str(), "%f", &float_val);
        } else {
            throw IncompatibleTypeError(coltype2str(type), coltype2str(new_type));
        }
        type = new_type;
    }

    // static Value col2Value(char *base, const ColMeta& meta) {
    //     Value value;
    //     switch (meta.type) {
    //         case TYPE_INT:
    //             value.set_int(*(int *) (base + meta.offset));
    //             break;
    //         case TYPE_FLOAT:
    //             value.set_float(*(float *) (base + meta.offset));
    //             break;
    //         case TYPE_STRING: {
    //             std::string str((char *) (base + meta.offset), meta.len);
    //             // 去掉末尾的'\0', 考虑无tailing-zero的情况
    //             str.resize(std::min(str.find('\0'), (size_t)meta.len));
    //             value.set_str(str);
    //             break;
    //         }
    //         default:
    //             throw InternalError("not implemented");
    //     }
    //     return value;
    // }

    void float2int(){
        assert(type == TYPE_FLOAT);
        int_val = (int)float_val;
        type = TYPE_INT;
    }

    void int2float(){
        assert(type == TYPE_INT);
        float_val = (float)int_val;
        type = TYPE_FLOAT;
    }

    bool try_cast_to(ColType target_type) {
        if (type == target_type) {
            return true;
        }
        if (type == TYPE_INT && target_type == TYPE_FLOAT) {
            int2float();
            return true;
        }
        if (type == TYPE_FLOAT && target_type == TYPE_INT) {
            float2int();
            return true;
        }
        return false;
    }

    bool operator==(const Value &rhs) const {
        if (type != rhs.type) throw IncompatibleTypeError(coltype2str(type), coltype2str(rhs.type));
        switch (type) {
            case TYPE_INT:
                return int_val == rhs.int_val;
            case TYPE_FLOAT:
                return float_val == rhs.float_val;
            case TYPE_STRING:
                return strncmp(str_val.c_str(), rhs.str_val.c_str(), str_len) == 0;
            default:
                throw InternalError("Unexpected value type");
        }
    }

    bool operator!=(const Value &rhs) const { return !(*this == rhs); }

    bool operator<(const Value &rhs) const {
        if (type != rhs.type) throw IncompatibleTypeError(coltype2str(type), coltype2str(rhs.type));
        switch (type) {
            case TYPE_INT:
                return int_val < rhs.int_val;
            case TYPE_FLOAT:
                return float_val < rhs.float_val;
            case TYPE_STRING:
                return strncmp(str_val.c_str(), rhs.str_val.c_str(), str_len) < 0;
            default:
                throw InternalError("Unexpected value type");
        }
    }

    bool operator>(const Value &rhs) const { return rhs < *this; }

    bool operator<=(const Value &rhs) const { return !(rhs < *this); }

    bool operator>=(const Value &rhs) const { return !(*this < rhs); }
};

enum CompOp { OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE, OP_IN };

class Query;
class Plan;
struct PortalStmt;
class AbstractExecutor;
struct ColMeta;

enum CondRhsType {
    RHS_VALUE,
    RHS_COL,
    RHS_SELECT,
    RHS_SET
};

struct Condition {
    TabCol lhs_col;   // left-hand side column
    CompOp op;        // comparison operator
    // TODO:下面这两个bool变量应该删除，用rhs_type代替
    bool is_rhs_val;  // true if right-hand side is a value (not a column)
    bool is_rhs_select; // true if right-hand side is a select statement
    TabCol rhs_col;   // right-hand side column
    Value rhs_val;    // right-hand side value
    std::shared_ptr<Query> rhs_query;
    std::shared_ptr<Plan> rhs_plan;
    std::shared_ptr<PortalStmt> rhs_portal;
    std::set<Value> rhs_set;
    CondRhsType rhs_type;
    std::vector<ColMeta>::const_iterator lhs_match_col;
    std::vector<ColMeta>::const_iterator rhs_match_col;

    bool check_condition(const Value& lhs, const Value& rhs) const {

        switch (op)
        {
        case OP_EQ: return lhs == rhs;
        case OP_NE: return lhs != rhs;
        case OP_LT: return lhs < rhs;
        case OP_GT: return lhs > rhs;
        case OP_LE: return lhs <= rhs;
        case OP_GE: return lhs >= rhs;
        case OP_IN: return rhs_set.find(lhs) != rhs_set.end();
        default:
            throw InternalError("Unexpected comparison operator");
        }
    }

    [[nodiscard]] bool eval_with_rvalue(const Value &lhs) const {
        assert(is_rhs_val);
        return check_condition(lhs, rhs_val);
    }

};

struct SetClause {
    TabCol lhs;
    Value rhs;
};