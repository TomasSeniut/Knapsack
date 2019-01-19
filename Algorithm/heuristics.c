#include <stdlib.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

stack_data highestValueWeightRatio(knapsack_global params)
{
    stack_data best;
    best.weight = 0;
    best.price = 0;
    InitializeArray(params.count, &best.taken);

    int i = 0;
    while (best.weight + params.items[i].weight <= params.weight)
    {
        best.price += params.items[i].price;
        best.weight += params.items[i].weight;
        best.taken[i] = 1;
        i++;
    }

    return best;
}