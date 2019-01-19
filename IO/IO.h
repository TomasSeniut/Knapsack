#ifndef KNAPSACK_IO_H
#define KNAPSACK_IO_H

#include "../DataStructure/DataStructure.h"

knapsack_global ReadGlobalData(const char *fileName);
void PrintResult(knapsack_global params, stack_data solution);


#endif //KNAPSACK_IO_H
