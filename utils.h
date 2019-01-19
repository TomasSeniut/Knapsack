#ifndef KNAPSACK_UTILS_H
#define KNAPSACK_UTILS_H

#include "DataStructure/DataStructure.h"

void InitializeArray(int n, char **array);
void InitializeAndCopyArray(int n, const char *from, char **to);
void CopyArray(int n, const char from[], char to[]);
void CopyItemsArray(int n, ItemWithIndex from[], Item items[], int indexes[]);
int itemIndexComparePriceWeightRatio(const void *a, const void *b);
int itemCompareWeight(const void *a, const void *b);


#endif //KNAPSACK_UTILS_H
