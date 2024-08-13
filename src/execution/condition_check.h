#pragma once
#include "common/common.h"
#include "system/sm_meta.h"

class ConditionCheck{
public:
    static void execute_sub_query(Condition& cond);

    static bool check_conditions(std::vector<Condition>& conds, const std::unique_ptr<RmRecord>& rec);

    static bool check_single_condition(Condition& cond, const std::unique_ptr<RmRecord>& rec);
    
private:
};
