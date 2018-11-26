//
// Created by tomas on 18.9.22.
//

#include <stdlib.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

stack_data highestValueWeightRatio(knapsack_global params)
{
    stack_data best;
    best.state.weight = 0;
    best.state.price = 0;
    InitializeArray(params.count, &best.taken);

    int i = 0;
    while (best.state.weight + params.items[i].weight <= params.weight)
    {
        best.state.price += params.items[i].price;
        best.state.weight += params.items[i].weight;
        best.taken[i] = 1;
        i++;
    }

    return best;
}