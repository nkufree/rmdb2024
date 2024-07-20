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

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include "common/common.h"
#include "errors.h"
#include "sm_defs.h"
#include "parser/ast.h"

/* 字段元数据 */
struct ColMeta {
    std::string tab_name;   // 字段所属表名称
    std::string name;       // 字段名称
    std::string alias;      // 字段别名
    ColType type;           // 字段类型
    int len;                // 字段长度
    int offset;             // 字段位于记录中的偏移量
    bool index;             /** unused */
    ast::AggregationType aggr = ast::NO_AGGR;

    friend std::ostream &operator<<(std::ostream &os, const ColMeta &col) {
        // ColMeta中有各个基本类型的变量，然后调用重载的这些变量的操作符<<（具体实现逻辑在defs.h）
        return os << col.tab_name << ' ' << col.name << ' ' << col.type << ' ' << col.len << ' ' << col.offset << ' '
                  << col.index;
    }

    friend std::istream &operator>>(std::istream &is, ColMeta &col) {
        return is >> col.tab_name >> col.name >> col.type >> col.len >> col.offset >> col.index;
    }

    Value to_value(char* base) const {
        Value value;
        switch (type) {
            case TYPE_INT:
                value.set_int(*(int *) (base + offset));
                break;
            case TYPE_FLOAT:
                value.set_float(*(float *) (base + offset));
                break;
            case TYPE_STRING: {
                std::string str((char *) (base + offset), len);
                // 去掉末尾的'\0', 考虑无tailing-zero的情况
                str.resize(std::min(str.find('\0'), (size_t)len));
                value.set_str(str);
                break;
            }
            default:
                throw InternalError("not implemented");
        }
        return value;
    }
};

/* 索引元数据 */
struct IndexMeta {
    std::string tab_name;           // 索引所属表名称
    int col_tot_len;                // 索引字段长度总和
    int col_num;                    // 索引字段数量
    std::vector<ColMeta> cols;      // 索引包含的字段

    friend std::ostream &operator<<(std::ostream &os, const IndexMeta &index) {
        os << index.tab_name << " " << index.col_tot_len << " " << index.col_num;
        for(auto& col: index.cols) {
            os << "\n" << col;
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &is, IndexMeta &index) {
        is >> index.tab_name >> index.col_tot_len >> index.col_num;
        for(int i = 0; i < index.col_num; ++i) {
            ColMeta col;
            is >> col;
            index.cols.push_back(col);
        }
        return is;
    }
};

/* 表元数据 */
struct TabMeta {
    std::string name;                   // 表名称
    std::vector<ColMeta> cols;          // 表包含的字段
    std::vector<IndexMeta> indexes;     // 表上建立的索引

    TabMeta(){}

    TabMeta(const TabMeta &other) {
        name = other.name;
        for(auto col : other.cols) cols.push_back(col);
    }

    /* 判断当前表中是否存在名为col_name的字段 */
    bool is_col(const std::string &col_name) const {
        auto pos = std::find_if(cols.begin(), cols.end(), [&](const ColMeta &col) { return col.name == col_name; });
        return pos != cols.end();
    }

    /* 判断当前表上是否建有指定索引，索引包含的字段为col_names */
    bool is_index(const std::vector<std::string>& col_names) const {
        for(auto& index: indexes) {
            if((size_t)index.col_num == col_names.size()) {
                size_t i = 0;
                for(; i < (size_t)index.col_num; ++i) {
                    if(index.cols[i].name.compare(col_names[i]) != 0)
                        break;
                }
                if(i == (size_t)index.col_num) return true;
            }
        }

        return false;
    }

    /* 根据字段名称集合获取索引元数据 */
    std::vector<IndexMeta>::iterator get_index_meta(const std::vector<std::string>& col_names) {
        for(auto index = indexes.begin(); index != indexes.end(); ++index) {
            if((*index).col_num != (int)col_names.size()) continue;
            auto& index_cols = (*index).cols;
            size_t i = 0;
            for(; i < col_names.size(); ++i) {
                if(index_cols[i].name.compare(col_names[i]) != 0) 
                    break;
            }
            if(i == col_names.size()) return index;
        }
        throw IndexNotFoundError(name, col_names);
    }

    /* 根据字段元数据集合获取索引元数据 */
    std::vector<IndexMeta>::iterator get_index_meta(const std::vector<ColMeta>& cols) {
        for(auto index = indexes.begin(); index != indexes.end(); ++index) {
            if((*index).col_num != (int)cols.size()) continue;
            auto& index_cols = (*index).cols;
            size_t i = 0;
            for(; i < cols.size(); ++i) {
                if(index_cols[i].name.compare(cols[i].name) != 0) 
                    break;
            }
            if(i == cols.size()) return index;
        }
        std::vector<std::string> col_names;
        for(auto& col: cols) {
            col_names.push_back(col.name);
        }
        throw IndexNotFoundError(name, col_names);
    }

    /* 根据字段名称获取字段元数据 */
    std::vector<ColMeta>::iterator get_col(const std::string &col_name) {
        auto pos = std::find_if(cols.begin(), cols.end(), [&](const ColMeta &col) { return col.name == col_name; });
        if (pos == cols.end()) {
            throw ColumnNotFoundError(col_name);
        }
        return pos;
    }

    int get_col_total_len() {
        return cols.back().offset + cols.back().len;
    }

    // 匹配索引，查找最长匹配的索引
    bool modify_and_check_index(std::vector<Condition>& conds, std::vector<std::string>& index_col_names) {
        if(conds.size() == 0) {
            if(indexes.size() == 0) return false;
            IndexMeta& index_meta = indexes[0];
            for(ColMeta& col: index_meta.cols) {
                index_col_names.push_back(col.name);
            }
            return true;
        }
        std::unordered_map<std::string, std::vector<int>> col_idx_map;
        for(size_t i = 0; i < conds.size(); i++) {
            if(col_idx_map.find(conds[i].lhs_col.col_name) == col_idx_map.end())
                col_idx_map[conds[i].lhs_col.col_name] = std::vector<int>{(int)i};
            else
                col_idx_map[conds[i].lhs_col.col_name].push_back(i);
        }
        // std::vector<Condition> new_conds;
        int max_match_len = 0;
        int max_match_idx = -1;
        for(size_t i = 0; i < indexes.size(); i++) {
            int match_len = 0;
            IndexMeta& index_meta = indexes[i];
            for(ColMeta& col: index_meta.cols) {
                if(col_idx_map.find(col.name) != col_idx_map.end()) {
                    match_len++;
                }
                else {
                    break;
                }
            }
            if(match_len > max_match_len) {
                max_match_len = match_len;
                max_match_idx = i;
            }
        }
        if(max_match_idx == -1) return false;
        IndexMeta& index_meta = indexes[max_match_idx];
        for(ColMeta& col: index_meta.cols) {
            index_col_names.push_back(col.name);
        }
        return true;
    }

    friend std::ostream &operator<<(std::ostream &os, const TabMeta &tab) {
        os << tab.name << '\n' << tab.cols.size() << '\n';
        for (auto &col : tab.cols) {
            os << col << '\n';  // col是ColMeta类型，然后调用重载的ColMeta的操作符<<
        }
        os << tab.indexes.size() << "\n";
        for (auto &index : tab.indexes) {
            os << index << "\n";
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &is, TabMeta &tab) {
        size_t n;
        is >> tab.name >> n;
        for (size_t i = 0; i < n; i++) {
            ColMeta col;
            is >> col;
            tab.cols.push_back(col);
        }
        is >> n;
        for(size_t i = 0; i < n; ++i) {
            IndexMeta index;
            is >> index;
            tab.indexes.push_back(index);
        }
        return is;
    }
};

// 注意重载了操作符 << 和 >>，这需要更底层同样重载TabMeta、ColMeta的操作符 << 和 >>
/* 数据库元数据 */
class DbMeta {
    friend class SmManager;

   private:
    std::string name_;                      // 数据库名称
    std::map<std::string, TabMeta> tabs_;   // 数据库中包含的表

   public:
    // DbMeta(std::string name) : name_(name) {}

    /* 判断数据库中是否存在指定名称的表 */
    bool is_table(const std::string &tab_name) const { return tabs_.find(tab_name) != tabs_.end(); }

    void SetTabMeta(const std::string &tab_name, const TabMeta &meta) {
        tabs_[tab_name] = meta;
    }

    /* 获取指定名称表的元数据 */
    TabMeta &get_table(const std::string &tab_name) {
        auto pos = tabs_.find(tab_name);
        if (pos == tabs_.end()) {
            throw TableNotFoundError(tab_name);
        }

        return pos->second;
    }

    // 重载操作符 <<
    friend std::ostream &operator<<(std::ostream &os, const DbMeta &db_meta) {
        os << db_meta.name_ << '\n' << db_meta.tabs_.size() << '\n';
        for (auto &entry : db_meta.tabs_) {
            os << entry.second << '\n';
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &is, DbMeta &db_meta) {
        size_t n;
        is >> db_meta.name_ >> n;
        for (size_t i = 0; i < n; i++) {
            TabMeta tab;
            is >> tab;
            db_meta.tabs_[tab.name] = tab;
        }
        return is;
    }
};
