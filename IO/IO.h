//
// Created by tomas on 18.9.20.
//

#ifndef KNAPSACK_IO_H
#define KNAPSACK_IO_H

#include "../DataStructure/DataStructure.h"

knapsack_global ReadGlobalData(const char *fileName);
void ReadItemsData(const char *fileName, Item items[]);


#endif //KNAPSACK_IO_H
