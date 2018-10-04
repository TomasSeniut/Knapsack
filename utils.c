//
// Created by tomas on 18.9.23.
//

#include <stdlib.h>
#include "DataStructure/DataStructure.h"

void InitializeArray(int n, int ** array) {
    *array = malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        (*array)[i] = 0;
    }
}

void CopyArray(int n, int from[], int to[]) {
    for (int i = 0; i < n; ++i) {
        to[i] = from[i];
    }
}

void InitializeAndCopyArray(int n, int *from, int **to) {
    *to = malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        (*to)[i] = from[i];
    }
}

void CopyItemsArray(int n, ItemWithIndex from[], Item items[], int indexes[])
{
    for (int i = 0; i < n; ++i) {
        items[i].price = from[i].price;
        items[i].weight = from[i].weight;
        indexes[i] = from[i].index;
    }
}

int itemIndexComparePriceWeightRatio(const void *a, const void *b)
{
    ItemWithIndex item1 = * ( (ItemWithIndex*) a );
    ItemWithIndex item2 = * ( (ItemWithIndex*) b );

    double r1 = (double)item1.price / item1.weight;
    double r2 = (double)item2.price / item2.weight;

    if ( r1 == r2 ) return 0;
    else if ( r1 > r2 ) return -1;
    else return 1;
}

int itemCompareWeight(const void *a, const void *b)
{
    Item item1 = * ( (Item*) a );
    Item item2 = * ( (Item*) b );

    double r1 = item1.weight;
    double r2 = item2.weight;

    if ( r1 == r2 ) return 0;
    else if ( r1 > r2 ) return -1;
    else return 1;
}
