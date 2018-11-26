//
// Created by tomas on 18.9.22.
//

#ifndef KNAPSACK_ALGORITHMS_H
#define KNAPSACK_ALGORITHMS_H

#include "../DataStructure/DataStructure.h"

stack_data highestValueWeightRatio(knapsack_global params);
stack_data simpleBranchAndBound(stack_data knowResult, knapsack_global params);
stack_data parallelBranchAndBound(stack_data knowResult, knapsack_global params);

#endif //KNAPSACK_ALGORITHMS_H
