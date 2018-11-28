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

    stack_data bestHeuristics = highestValueWeightRatio(params);

    double start = omp_get_wtime();
    stack_data knapsack = parallelBranchAndBound(bestHeuristics, params);
    double duration = omp_get_wtime() - start;

    printf("It took %f seconds for algorithm.\n", duration);

    PrintResult(params, knapsack);

    return 0;
}