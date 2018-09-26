#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "DataStructure/DataStructure.h"
#include "Algorithm/algorithms.h"
#include "IO/IO.h"
#include "utils.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: ./Knapsack problemFileName\n");
        return 1;
    }

    char* dataFileName = argv[1];

    knapsack_global params = ReadGlobalData(dataFileName);

    Item items[params.items];
    ReadItemsData(dataFileName, items);

    qsort(items, (size_t) params.items, sizeof(Item), itemComparePriceWeightRatio);
    stack_data bestHeuristics = highestValueWeightRatio(params, items);
    printf("Best solution by heuristics:\n Knapsack price: %d, weight: %d\n", bestHeuristics.state.price, bestHeuristics.state.weight);

    double start = omp_get_wtime();
    stack_data knapsack = simpleBranchAndBound(bestHeuristics, params, items);
    double duration = omp_get_wtime() - start;
    printf("It took %f seconds for algorithm.\n", duration);

    printf("Solution from B&B:\n Knapsack price: %d, weight: %d\n", knapsack.state.price, knapsack.state.weight);
    printf("Taken items:");
    for (int i = 0; i < params.items; ++i) {
        printf(" %d:%d", i, knapsack.taken[i]);
    }
    printf("\n");

    return 0;
}