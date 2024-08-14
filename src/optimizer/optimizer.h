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

#include <map>

#include "errors.h"
#include "execution/execution.h"
#include "parser/parser.h"
#include "system/sm.h"
#include "common/context.h"
#include "transaction/transaction_manager.h"
#include "planner.h"
#include "plan.h"

class Optimizer {
   private:
    SmManager *sm_manager_;
    Planner *planner_;

   public:
    Optimizer(SmManager *sm_manager,  Planner *planner) 
        : sm_manager_(sm_manager),  planner_(planner)
        {}
    
    std::shared_ptr<Plan> plan_query(std::shared_ptr<Query> query, Context *context) {
        // 先解析query的条件，对子查询进行解析
        for(Condition& cond : query->conds) {
            if(cond.rhs_type == CondRhsType::RHS_SELECT) {
                cond.rhs_plan = plan_query(cond.rhs_query, context);
            }
        }
        for(Condition& cond : query->having_conds) {
            if(cond.rhs_type == CondRhsType::RHS_SELECT) {
                cond.rhs_plan = plan_query(cond.rhs_query, context);
            }
        }
        if (auto x = std::dynamic_pointer_cast<ast::Help>(query->parse)) {
            // help;
            return std::make_shared<OtherPlan>(T_Help, std::string());
        } else if (auto x = std::dynamic_pointer_cast<ast::SetOutputFile>(query->parse)) {
            // set output_file off
            return std::make_shared<OtherPlan>(T_SetOutputFile, std::string());
        } else if (auto x = std::dynamic_pointer_cast<ast::ShowTables>(query->parse)) {
            // show tables;
            return std::make_shared<OtherPlan>(T_ShowTable, std::string());
        } else if (auto x = std::dynamic_pointer_cast<ast::DescTable>(query->parse)) {
            // desc table;
            return std::make_shared<OtherPlan>(T_DescTable, x->tab_name);
        } else if (auto x = std::dynamic_pointer_cast<ast::ShowIndex>(query->parse)) {
            // show index;
            return std::make_shared<OtherPlan>(T_ShowIndex, x->tab_name);
        } else if (auto x = std::dynamic_pointer_cast<ast::TxnBegin>(query->parse)) {
            // begin;
            return std::make_shared<OtherPlan>(T_Transaction_begin, std::string());
        } else if (auto x = std::dynamic_pointer_cast<ast::TxnAbort>(query->parse)) {
            // abort;
            return std::make_shared<OtherPlan>(T_Transaction_abort, std::string());
        } else if (auto x = std::dynamic_pointer_cast<ast::TxnCommit>(query->parse)) {
            // commit;
            return std::make_shared<OtherPlan>(T_Transaction_commit, std::string());
        } else if (auto x = std::dynamic_pointer_cast<ast::TxnRollback>(query->parse)) {
            // rollback;
            return std::make_shared<OtherPlan>(T_Transaction_rollback, std::string());
        } else if (auto x = std::dynamic_pointer_cast<ast::SetStmt>(query->parse)) {
            // Set Knob Plan
            return std::make_shared<SetKnobPlan>(x->set_knob_type_, x->bool_val_);
        } else if (auto x = std::dynamic_pointer_cast<ast::CreateStaticCheckpoint>(query->parse)) {
            // create static_checkpoint;
            return std::make_shared<OtherPlan>(T_Checkpoint, std::string());
        } else if (auto x = std::dynamic_pointer_cast<ast::LoadStmt>(query->parse)) {
            // load
            return std::make_shared<LoadPlan>(T_LoadFile, x->file_name, x->tab_name);
        } else {
            return planner_->do_planner(query, context);
        }
    }

};
