//
// Created by tomas on 18.9.23.
//

#include <omp.h>
#include <stdio.h>
#include "IO/IO.h"
#include "Algorithm/algorithms.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: ./Knapsack problemFileName\n");
        return 1;
    }

    char* dataFileName = argv[1];

    knapsack_global params = ReadGlobalData(dataFileName);

    ItemWithIndex items_index[params.items];
    ReadItemsData(dataFileName, items_index);

    qsort(items_index, (size_t) params.items, sizeof(ItemWithIndex), itemIndexComparePriceWeightRatio);

    Item items[params.items];
    int indexes[params.items];
    CopyItemsArray(params.items, items_index, items, indexes);

    stack_data bestHeuristics = highestValueWeightRatio(params, items);
    printf("Best solution by heuristics:\n Knapsack price: %d, weight: %d\n", bestHeuristics.state.price, bestHeuristics.state.weight);

    double start = omp_get_wtime();
    stack_data knapsack = simpleBranchAndBound(bestHeuristics, params, items);
    double duration = omp_get_wtime() - start;
    PrintResult(params, knapsack, indexes);

    printf("It took %f seconds for algorithm.\n", duration);

    PrintResult(params, knapsack);

    return 0;
}