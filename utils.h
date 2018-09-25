//
// Created by tomas on 18.9.22.
//

#ifndef KNAPSACK_UTILS_H
#define KNAPSACK_UTILS_H

#include "DataStructure/DataStructure.h"

void InitializeArray(int n, int **array);
void InitializeAndCopyArray(int n, int *from, int **to);
void CopyArray(int n, int from[], int to[]);
int itemComparePriceWeightRatio(const void *a, const void *b);
int itemCompareWeight(const void *a, const void *b);


#endif //KNAPSACK_UTILS_H
