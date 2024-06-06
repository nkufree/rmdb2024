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


struct TabCol {
    std::string tab_name;
    std::string col_name;
    std::string alias;

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
            memset(raw->data, 0, len);
            memcpy(raw->data, str_val.c_str(), str_val.size());
        }
    }

    void value_cast(ColType new_type) {
        if (type == new_type) return;
        if (type == TYPE_INT && new_type == TYPE_FLOAT) {
            float_val = int_val;
        } else if (type == TYPE_FLOAT && new_type == TYPE_INT) {
            int_val = float_val;
        } else {
            throw IncompatibleTypeError(coltype2str(type), coltype2str(new_type));
        }
        type = new_type;
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

enum CompOp { OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE };

struct Condition {
    TabCol lhs_col;   // left-hand side column
    CompOp op;        // comparison operator
    bool is_rhs_val;  // true if right-hand side is a value (not a column)
    TabCol rhs_col;   // right-hand side column
    Value rhs_val;    // right-hand side value

    bool check_condition(const Value& lhs, const Value& rhs) const {

        switch (op)
        {
        case OP_EQ: return lhs == rhs;
        case OP_NE: return lhs != rhs;
        case OP_LT: return lhs < rhs;
        case OP_GT: return lhs > rhs;
        case OP_LE: return lhs <= rhs;
        case OP_GE: return lhs >= rhs;
        default:
            throw InternalError("Unexpected comparison operator");
        }
    }
};

struct SetClause {
    TabCol lhs;
    Value rhs;
};