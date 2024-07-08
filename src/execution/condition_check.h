#pragma once
#include "common/common.h"
#include "system/sm_meta.h"

class ConditionCheck{
public:
    static void execute_sub_query(Condition& cond);

    static bool check_conditions(const std::vector<Condition>& conds, const std::vector<ColMeta>& cols, const std::unique_ptr<RmRecord>& rec);

    static bool check_single_condition(const Condition& cond, const std::vector<ColMeta>& cols, const std::unique_ptr<RmRecord>& rec);
    
private:
};
