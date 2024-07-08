/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "analyze.h"

/**
 * @description: 分析器，进行语义分析和查询重写，需要检查不符合语义规定的部分
 * @param {shared_ptr<ast::TreeNode>} parse parser生成的结果集
 * @return {shared_ptr<Query>} Query 
 */
std::shared_ptr<Query> Analyze::do_analyze(std::shared_ptr<ast::TreeNode> parse)
{
    std::shared_ptr<Query> query = std::make_shared<Query>();
    if (auto x = std::dynamic_pointer_cast<ast::SelectStmt>(parse))
    {
        // 处理表名
        query->tables = std::move(x->tabs);
        /** TODO: 检查表是否存在 */
        for (auto &tab_name : query->tables) {
            if (!sm_manager_->db_.is_table(tab_name)) {
                throw TableNotFoundError(tab_name);
            }
        }
        bool has_aggr = false;
        bool has_non_aggr = false;
        // 处理target list，再target list中添加上表名，例如 a.id
        for (auto &sv_sel_col : x->cols) {
            if (x->group != nullptr && x->group->cols.empty()) {
                throw AmbiguousColumnError("must have group by clause when using aggregate function");
            }
            if (sv_sel_col->aggr_type != ast::NO_AGGR) {
                has_aggr = true;
            } else {
                has_non_aggr = true;
            }
            if (has_aggr && has_non_aggr && x->group == nullptr) {
                throw AmbiguousColumnError("SELECT list contains both an aggregated and a non-aggregated column without GROUP BY clause");
            }
            //新增alias的
            TabCol sel_col = {.tab_name = sv_sel_col->tab_name, .col_name = sv_sel_col->col_name,.alias = sv_sel_col->alias,.aggr = sv_sel_col->aggr_type};
            query->cols.push_back(sel_col);
        }

        query->has_aggr = has_aggr;
        std::vector<ColMeta> all_cols;
        get_all_cols(query->tables, all_cols);
        if (query->cols.empty()) {
            // select all columns
            for (auto &col : all_cols) {
                TabCol sel_col = {.tab_name = col.tab_name, .col_name = col.name,.alias = col.alias, .aggr = ast::NO_AGGR};
                query->cols.push_back(sel_col);
            }
        } else {
            // infer table name from column name
            for (auto &sel_col : query->cols) {
                sel_col = check_column(all_cols, sel_col);  // 列元数据校验
            }
        }
        // group by 子句处理
        if (x->group != nullptr) {
            for (auto &sv_group_col : x->group->cols) {
                TabCol group_col = {
                    .tab_name = sv_group_col->tab_name,
                    .col_name = sv_group_col->col_name,
                    .alias = sv_group_col->alias,
                    .aggr = sv_group_col->aggr_type
                };
                query->group_cols.push_back(group_col);
            }
            for (auto &sv_having_cond : x->group->conds) {
                // having 和 where 一样的处理
                get_clause(x->group->conds, query->having_conds);
                check_clause(query->tables, query->having_conds, true);
            }

            // 检查合法性、给 group by 子句中的列补全表名
            for (auto &group_col : query->group_cols) {
                group_col = check_column(all_cols, group_col);
            }

            // SELECT 列表中不能出现没有在 GROUP BY 子句中的非聚集列
            for (auto &sel_col : query->cols) {
                if (sel_col.aggr == ast::NO_AGGR) {
                    bool found = false;
                    for (auto &group_col : query->group_cols) {
                        if (sel_col.col_name == group_col.col_name && sel_col.tab_name == group_col.tab_name) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        throw AmbiguousColumnError("SELECT list contains non-aggregated column that is not in GROUP BY clause");
                    }
                }
            }
        }
        // 处理on条件
        get_clause(x->on_conds, query->on_conds);
        check_clause(query->tables, query->on_conds, false);
        //处理where条件
        get_clause(x->conds, query->conds);
        check_clause(query->tables, query->conds, false);
    } else if (auto x = std::dynamic_pointer_cast<ast::UpdateStmt>(parse)) {
        /** TODO: (done)*/
        for(auto& sv_set : x->set_clauses){
            SetClause set_clause;
            set_clause.lhs = {.tab_name = x->tab_name, .col_name = sv_set->col_name};
            set_clause.rhs = convert_sv_value(sv_set->val);
            query->set_clauses.push_back(set_clause);
        }
        get_clause(x->conds, query->conds);
        check_clause({x->tab_name}, query->conds, false); 
    } else if (auto x = std::dynamic_pointer_cast<ast::DeleteStmt>(parse)) {
        //处理where条件
        get_clause(x->conds, query->conds);
        check_clause({x->tab_name}, query->conds, false);        
    } else if (auto x = std::dynamic_pointer_cast<ast::InsertStmt>(parse)) {
        // 处理insert 的values值
        for (auto &sv_val_list : x->vals) {
            std::vector<Value> values;
            for(auto &sv_val : sv_val_list) {
                values.push_back(convert_sv_value(sv_val));
            }
            query->values.push_back(values);
        }
    } else {
        // do nothing
    }
    query->parse = std::move(parse);
    return query;
}



TabCol Analyze::check_column(const std::vector<ColMeta> &all_cols, TabCol target) {
    if (target.tab_name.empty()) {
        // Table name not specified, infer table name from column name
        std::string tab_name;
        for (auto &col : all_cols) {
            if (col.name == target.col_name) {
                if (!tab_name.empty()) {
                    throw AmbiguousColumnError(target.col_name);
                }
                tab_name = col.tab_name;
            }
        }
        if (tab_name.empty()) {
            if (target.aggr == ast::AGGR_TYPE_COUNT && target.col_name == "*") {
                return target;
            }
            throw ColumnNotFoundError(target.col_name);
        }

        target.tab_name = tab_name;
    } else {
        /** TODO: Make sure target column exists */
        
    }
    return target;
}

void Analyze::get_all_cols(const std::vector<std::string> &tab_names, std::vector<ColMeta> &all_cols) {
    for (auto &sel_tab_name : tab_names) {
        // 这里db_不能写成get_db(), 注意要传指针
        const auto &sel_tab_cols = sm_manager_->db_.get_table(sel_tab_name).cols;
        all_cols.insert(all_cols.end(), sel_tab_cols.begin(), sel_tab_cols.end());
    }
}

void Analyze::get_clause(const std::vector<std::shared_ptr<ast::BinaryExpr>> &sv_conds, std::vector<Condition> &conds) {
    conds.clear();
    for (auto &expr : sv_conds) {
        Condition cond;
        cond.lhs_col = {
            .tab_name = expr->lhs->tab_name, 
            .col_name = expr->lhs->col_name,
            .alias = expr->lhs->alias,
            .aggr = expr->lhs->aggr_type
        };
        cond.op = convert_sv_comp_op(expr->op);
        if (auto rhs_val = std::dynamic_pointer_cast<ast::Value>(expr->rhs)) {
            cond.is_rhs_val = true;
            cond.is_rhs_select = false;
            cond.rhs_type = CondRhsType::RHS_VALUE;
            cond.rhs_val = convert_sv_value(rhs_val);
        } else if (auto rhs_col = std::dynamic_pointer_cast<ast::Col>(expr->rhs)) {
            cond.is_rhs_val = false;
            cond.is_rhs_select = false;
            cond.rhs_col = {
                .tab_name = rhs_col->tab_name,
                .col_name = rhs_col->col_name,
                .alias = rhs_col->alias,
                .aggr = rhs_col->aggr_type
            };
            cond.rhs_type = CondRhsType::RHS_COL;
        }
        else if(auto rhs_col = std::dynamic_pointer_cast<ast::SubQueryStmt>(expr->rhs)){
            cond.is_rhs_val = false;
            cond.is_rhs_select = true;
            cond.rhs_query = this->do_analyze(rhs_col->subquery);
            cond.rhs_type = CondRhsType::RHS_SELECT;
        }
        else if(auto rhs_col = std::dynamic_pointer_cast<ast::ValueList>(expr->rhs)){
            cond.is_rhs_val = false;
            cond.is_rhs_select = false;
            for(auto& val: rhs_col->vals){
                cond.rhs_set.insert(convert_sv_value(val));
            }
            cond.rhs_type = CondRhsType::RHS_SET;
        }
        else {
            throw InternalError("Unexpected rhs type");
        }
        conds.push_back(cond);
    }
}

// on
void Analyze::check_on_clause(const std::vector<std::string> &tab_names, std::vector<Condition> &conds) {
    // auto all_cols = get_all_cols(tab_names);
    std::vector<ColMeta> all_cols;
    get_all_cols(tab_names, all_cols);
    // Get raw values in where clause
    for (auto &cond : conds) {
        // Infer table name from column name
        cond.lhs_col = check_column(all_cols, cond.lhs_col);
        if (!cond.is_rhs_val) {
            cond.rhs_col = check_column(all_cols, cond.rhs_col);
        }
        TabMeta &lhs_tab = sm_manager_->db_.get_table(cond.lhs_col.tab_name);
        auto lhs_col = lhs_tab.get_col(cond.lhs_col.col_name);
        ColType lhs_type = lhs_col->type;
        ColType rhs_type;
          if (cond.is_rhs_val) {
                switch (cond.rhs_val.type)
                {
                case TYPE_INT:
                    cond.rhs_val.init_raw(sizeof(int));
                    break;
                case TYPE_FLOAT:
                    cond.rhs_val.init_raw(sizeof(float));
                    break;
                case TYPE_STRING:
                    cond.rhs_val.init_raw(cond.rhs_val.str_val.size());
                    break;
                default:
                    break;
                }
                rhs_type = cond.rhs_val.type;
            }
     else {
            throw InternalError("Unexpected on clause");
        }
        if (!value_type_match(lhs_type, rhs_type)) {
            throw IncompatibleTypeError(coltype2str(lhs_type), coltype2str(rhs_type));
        }
    }
}

// where
void Analyze::check_clause(const std::vector<std::string> &tab_names, std::vector<Condition> &conds, bool is_having) {
    // auto all_cols = get_all_cols(tab_names);
    std::vector<ColMeta> all_cols;
    get_all_cols(tab_names, all_cols);
    // Get raw values in where clause
    for (auto &cond : conds) {
        // Infer table name from column name
        if (!is_having && (cond.lhs_col.aggr != ast::NO_AGGR) || (cond.rhs_type == CondRhsType::RHS_COL && cond.rhs_col.aggr != ast::NO_AGGR)) {
            throw AmbiguousColumnError("aggregate functions are not allowed in WHERE clause");
        } 

        cond.lhs_col = check_column(all_cols, cond.lhs_col);
        if (cond.rhs_type == CondRhsType::RHS_COL) {
            cond.rhs_col = check_column(all_cols, cond.rhs_col);
        }
        if (cond.lhs_col.aggr == ast::AGGR_TYPE_COUNT && cond.lhs_col.col_name == "*") {
            ColType lhs_type = TYPE_INT;
            ColType rhs_type;
            if (cond.is_rhs_val) {
                cond.rhs_val.init_raw(sizeof(int));
                rhs_type = cond.rhs_val.type;
            } else {
                rhs_type = sm_manager_->db_.get_table(cond.rhs_col.tab_name).get_col(cond.rhs_col.col_name)->type;
            }
            if(!value_type_match(lhs_type, rhs_type)){
                throw IncompatibleTypeError(coltype2str(lhs_type), coltype2str(rhs_type));
            }

        } else {
            TabMeta &lhs_tab = sm_manager_->db_.get_table(cond.lhs_col.tab_name);
            auto lhs_col = lhs_tab.get_col(cond.lhs_col.col_name);
            ColType lhs_type = lhs_col->type;
            ColType rhs_type;
            if (cond.rhs_type == CondRhsType::RHS_VALUE)  {
                cond.rhs_val.init_raw(lhs_col->len);
                rhs_type = cond.rhs_val.type;
            } else if(cond.rhs_type == CondRhsType::RHS_COL){
                TabMeta &rhs_tab = sm_manager_->db_.get_table(cond.rhs_col.tab_name);
                auto rhs_col = rhs_tab.get_col(cond.rhs_col.col_name);
                rhs_type = rhs_col->type;
            }
            else if(cond.rhs_type == CondRhsType::RHS_SELECT){// select
                //TODO: 检查select语句是否合法
                auto query = cond.rhs_query;
                if(query->group_cols.size() > 0){
                    throw AmbiguousColumnError("subquery must return only one column");
                }
                if(query->cols.size() > 1){
                    throw AmbiguousColumnError("subquery must return only one column");
                }
            }
            if ((cond.rhs_type == CondRhsType::RHS_COL || cond.rhs_type == CondRhsType::RHS_VALUE) && !value_type_match(lhs_type, rhs_type)) {
                throw IncompatibleTypeError(coltype2str(lhs_type), coltype2str(rhs_type));
            }
        }
    }
}


Value Analyze::convert_sv_value(const std::shared_ptr<ast::Value> &sv_val) {
    Value val;
    if (auto int_lit = std::dynamic_pointer_cast<ast::IntLit>(sv_val)) {
        val.set_int(int_lit->val);
    } else if (auto float_lit = std::dynamic_pointer_cast<ast::FloatLit>(sv_val)) {
        val.set_float(float_lit->val);
    } else if (auto str_lit = std::dynamic_pointer_cast<ast::StringLit>(sv_val)) {
        val.set_str(str_lit->val);
    } else {
        throw InternalError("Unexpected sv value type");
    }
    return val;
}

CompOp Analyze::convert_sv_comp_op(ast::SvCompOp op) {
    std::map<ast::SvCompOp, CompOp> m = {
        {ast::SV_OP_EQ, OP_EQ}, {ast::SV_OP_NE, OP_NE}, {ast::SV_OP_LT, OP_LT},
        {ast::SV_OP_GT, OP_GT}, {ast::SV_OP_LE, OP_LE}, {ast::SV_OP_GE, OP_GE},
        {ast::SV_OP_IN, OP_IN}
    };
    return m.at(op);
}

// 判断两个类型是否可以比较
bool Analyze::value_type_match(ColType type1, ColType type2) {
    if (type1 == type2) {
        return true;
    }
    if (type1 == ColType::TYPE_INT && type2 == ColType::TYPE_FLOAT) {
        return true;
    }
    if (type1 == ColType::TYPE_FLOAT && type2 == ColType::TYPE_INT) {
        return true;
    }
    return false;
}

void Analyze::check_fix_clause(const std::string &tab_name, std::vector<SetClause>& clauses){
    TabMeta table = sm_manager_->db_.get_table(tab_name);
    for(auto& clause: clauses){
        table.is_col(clause.lhs.col_name);  
        ColType lhs_type = table.get_col(clause.lhs.col_name)->type;
        ColType rhs_type = clause.rhs.type;
        if(!value_type_match(lhs_type, rhs_type)){   
            throw IncompatibleTypeError(coltype2str(lhs_type), coltype2str(rhs_type));
        }
        if(lhs_type == TYPE_INT && rhs_type == TYPE_FLOAT){
            clause.rhs.float2int();
        }else if(lhs_type == TYPE_FLOAT && rhs_type == TYPE_INT){
            clause.rhs.int2float();
        }
    }
}
