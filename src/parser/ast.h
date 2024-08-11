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

#include <vector>
#include <string>
#include <memory>

enum JoinType {
    INNER_JOIN, LEFT_JOIN, RIGHT_JOIN, FULL_JOIN
};
namespace ast {

enum SvType {
    SV_TYPE_INT, SV_TYPE_FLOAT, SV_TYPE_STRING, SV_TYPE_BOOL
};

enum SvCompOp {
    SV_OP_EQ, SV_OP_NE, SV_OP_LT, SV_OP_GT, SV_OP_LE, SV_OP_GE, SV_OP_IN
};

enum OrderByDir {
    OrderBy_DEFAULT,
    OrderBy_ASC,
    OrderBy_DESC
};

enum SetKnobType {
    EnableNestLoop, EnableSortMerge
};

enum AggregationType {
    NO_AGGR,
    AGGR_TYPE_COUNT,
    AGGR_TYPE_MAX,
    AGGR_TYPE_MIN,
    AGGR_TYPE_SUM
};

// Base class for tree nodes
struct TreeNode {
    virtual ~TreeNode() = default;  // enable polymorphism
};

struct Help : public TreeNode {
};

struct SetOutputFile : public TreeNode {
};

struct ShowTables : public TreeNode {
};

struct TxnBegin : public TreeNode {
};

struct TxnCommit : public TreeNode {
};

struct TxnAbort : public TreeNode {
};

struct TxnRollback : public TreeNode {
};

struct TypeLen : public TreeNode {
    SvType type;
    int len;

    TypeLen(SvType type_, int len_) : type(type_), len(len_) {}
};

struct Field : public TreeNode {
};

struct ColDef : public Field {
    std::string col_name;
    std::shared_ptr<TypeLen> type_len;

    ColDef(std::string col_name_, std::shared_ptr<TypeLen> type_len_) :
            col_name(std::move(col_name_)), type_len(std::move(type_len_)) {}
};

struct CreateTable : public TreeNode {
    std::string tab_name;
    std::vector<std::shared_ptr<Field>> fields;

    CreateTable(std::string tab_name_, std::vector<std::shared_ptr<Field>> fields_) :
            tab_name(std::move(tab_name_)), fields(std::move(fields_)) {}
};

struct DropTable : public TreeNode {
    std::string tab_name;

    DropTable(std::string tab_name_) : tab_name(std::move(tab_name_)) {}
};

struct DescTable : public TreeNode {
    std::string tab_name;

    DescTable(std::string tab_name_) : tab_name(std::move(tab_name_)) {}
};

struct ShowIndex : public TreeNode {
    std::string tab_name;

    ShowIndex(std::string tab_name_) : tab_name(std::move(tab_name_)) {}
};

struct CreateIndex : public TreeNode {
    std::string tab_name;
    std::vector<std::string> col_names;

    CreateIndex(std::string tab_name_, std::vector<std::string> col_names_) :
            tab_name(std::move(tab_name_)), col_names(std::move(col_names_)) {}
};

struct DropIndex : public TreeNode {
    std::string tab_name;
    std::vector<std::string> col_names;

    DropIndex(std::string tab_name_, std::vector<std::string> col_names_) :
            tab_name(std::move(tab_name_)), col_names(std::move(col_names_)) {}
};

struct LoadStmt : public TreeNode {
    std::string file_name;
    std::string tab_name;

    LoadStmt(std::string file_name_, std::string tab_name_) :
            file_name(std::move(file_name_)), tab_name(std::move(tab_name_)) {}
};

struct CreateStaticCheckpoint : public TreeNode {
};

struct Expr : public TreeNode {
};

struct Value : public Expr {
};

struct IntLit : public Value {
    int val;

    IntLit(int val_) : val(val_) {}
};

struct FloatLit : public Value {
    float val;

    FloatLit(float val_) : val(val_) {}
};

struct StringLit : public Value {
    std::string val;

    StringLit(std::string val_) : val(std::move(val_)) {}
};

struct BoolLit : public Value {
    bool val;

    BoolLit(bool val_) : val(val_) {}
};

struct ValueList : public Expr {
    std::vector<std::shared_ptr<Value>> vals;

    ValueList(std::vector<std::shared_ptr<Value>> vals_) : vals(std::move(vals_)) {}
};

struct Col : public Expr {
    std::string tab_name;
    std::string col_name;
    std::string alias; // 默认情况下为空
    AggregationType aggr_type = NO_AGGR;

    Col(std::string tab_name_, std::string col_name_) :
            tab_name(std::move(tab_name_)), col_name(std::move(col_name_)) {}
    Col(std::string tab_name_, std::string col_name_, std::string alias_) :
            tab_name(std::move(tab_name_)), col_name(std::move(col_name_)), alias(std::move(alias_)) {}

};

struct SetClause : public TreeNode {
    std::string col_name;
    std::shared_ptr<Value> val;

    SetClause(std::string col_name_, std::shared_ptr<Value> val_) :
            col_name(std::move(col_name_)), val(std::move(val_)) {}
};

struct BinaryExpr : public TreeNode {
    std::shared_ptr<Col> lhs;
    SvCompOp op;
    std::shared_ptr<Expr> rhs;

    BinaryExpr(std::shared_ptr<Col> lhs_, SvCompOp op_, std::shared_ptr<Expr> rhs_) :
            lhs(std::move(lhs_)), op(op_), rhs(std::move(rhs_)) {}
};

struct OrderBy : public TreeNode
{
    std::shared_ptr<Col> cols;
    OrderByDir orderby_dir;
    OrderBy( std::shared_ptr<Col> cols_, OrderByDir orderby_dir_) :
       cols(std::move(cols_)), orderby_dir(std::move(orderby_dir_)) {}
};

struct GroupBy : public TreeNode
{
    std::vector<std::shared_ptr<Col>> cols;
    std::vector<std::shared_ptr<BinaryExpr>> conds;

    GroupBy(std::vector<std::shared_ptr<Col>> cols_) :
       cols(std::move(cols_)) {}
    GroupBy(std::vector<std::shared_ptr<Col>> cols_, std::vector<std::shared_ptr<BinaryExpr>> conds_) :
       cols(std::move(cols_)), conds(std::move(conds_)) {}
};

struct InsertStmt : public TreeNode {
    std::string tab_name;
    std::vector<std::vector<std::shared_ptr<Value>>> vals;

    InsertStmt(std::string tab_name_, std::vector<std::vector<std::shared_ptr<Value>>> vals_) :
            tab_name(std::move(tab_name_)), vals(std::move(vals_)) {}
};

struct DeleteStmt : public TreeNode {
    std::string tab_name;
    std::vector<std::shared_ptr<BinaryExpr>> conds;

    DeleteStmt(std::string tab_name_, std::vector<std::shared_ptr<BinaryExpr>> conds_) :
            tab_name(std::move(tab_name_)), conds(std::move(conds_)) {}
};

struct UpdateStmt : public TreeNode {
    std::string tab_name;
    std::vector<std::shared_ptr<SetClause>> set_clauses;
    std::vector<std::shared_ptr<BinaryExpr>> conds;

    UpdateStmt(std::string tab_name_,
               std::vector<std::shared_ptr<SetClause>> set_clauses_,
               std::vector<std::shared_ptr<BinaryExpr>> conds_) :
            tab_name(std::move(tab_name_)), set_clauses(std::move(set_clauses_)), conds(std::move(conds_)) {}
};

struct JoinExpr : public TreeNode {
    std::string left;
    std::string right;
    std::vector<std::shared_ptr<BinaryExpr>> conds;
    JoinType type;

    JoinExpr(std::string left_, std::string right_,
               std::vector<std::shared_ptr<BinaryExpr>> conds_, JoinType type_) :
            left(std::move(left_)), right(std::move(right_)), conds(std::move(conds_)), type(type_) {}
};

struct SelectStmt : public TreeNode {
    std::vector<std::shared_ptr<Col>> cols;
    std::vector<std::string> tabs;
    std::vector<std::shared_ptr<BinaryExpr>> on_conds;
    std::vector<std::shared_ptr<BinaryExpr>> conds;
    std::vector<std::shared_ptr<JoinExpr>> jointree;

    bool has_sort;
    std::shared_ptr<OrderBy> order;
    std::shared_ptr<GroupBy> group;

    SelectStmt(std::vector<std::shared_ptr<Col>> cols_,
               std::vector<std::string> tabs_,
               std::vector<std::shared_ptr<BinaryExpr>> on_conds_,
               std::vector<std::shared_ptr<BinaryExpr>> conds_,
               std::shared_ptr<OrderBy> order_,
               std::shared_ptr<GroupBy> group_) :
            cols(std::move(cols_)), tabs(std::move(tabs_)),on_conds(std::move(on_conds_)), conds(std::move(conds_)), 
            group(std::move(group_)), order(std::move(order_)) {has_sort = (bool)order;}
};


struct SubQueryStmt: public Expr {
    std::shared_ptr<SelectStmt> subquery;

    SubQueryStmt(std::shared_ptr<SelectStmt> subquery_) :
            subquery(std::move(subquery_)) {}
};

// set enable_nestloop
struct SetStmt : public TreeNode {
    SetKnobType set_knob_type_;
    bool bool_val_;

    SetStmt(SetKnobType &type, bool bool_value) : 
        set_knob_type_(type), bool_val_(bool_value) { }
};

// Semantic value
struct SemValue {
    int sv_int;
    float sv_float;
    std::string sv_str;
    bool sv_bool;
    OrderByDir sv_orderby_dir;
    AggregationType sv_aggr_type;

    std::vector<std::string> sv_strs;

    std::shared_ptr<TreeNode> sv_node;

    SvCompOp sv_comp_op;

    std::shared_ptr<TypeLen> sv_type_len;

    std::shared_ptr<Field> sv_field;
    std::vector<std::shared_ptr<Field>> sv_fields;

    std::shared_ptr<Expr> sv_expr;

    std::shared_ptr<Value> sv_val;
    std::vector<std::shared_ptr<Value>> sv_vals;
    std::vector<std::vector<std::shared_ptr<Value>>> sv_vals_list;

    std::shared_ptr<Col> sv_col;
    std::vector<std::shared_ptr<Col>> sv_cols;

    std::shared_ptr<SetClause> sv_set_clause;
    std::vector<std::shared_ptr<SetClause>> sv_set_clauses;

    std::shared_ptr<BinaryExpr> sv_cond;
    std::vector<std::shared_ptr<BinaryExpr>> sv_conds;

    std::shared_ptr<OrderBy> sv_orderby;

    std::shared_ptr<GroupBy> sv_groupby;

    SetKnobType sv_setKnobType;
};

extern std::shared_ptr<ast::TreeNode> parse_tree;

}

#define YYSTYPE ast::SemValue
