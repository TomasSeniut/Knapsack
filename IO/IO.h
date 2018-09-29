//
// Created by tomas on 18.9.20.
//

#ifndef KNAPSACK_IO_H
#define KNAPSACK_IO_H

#include "../DataStructure/DataStructure.h"

knapsack_global ReadGlobalData(const char *fileName);
void ReadItemsData(const char *fileName, ItemWithIntedexes items[]);
void PrintResult(knapsack_global params, stack_data solution, int correctIndexes[]);


#endif //KNAPSACK_IO_H
