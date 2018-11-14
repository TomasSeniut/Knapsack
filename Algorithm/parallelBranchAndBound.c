//
// Created by tomas on 18.11.14.
//

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"
#include "../DataStructure/ParallelStack.h"

static int geKnapsackBound(knapsack_global params, stack_data problem);

static int CanNotAddMoreItems(knapsack_global params, const int taken[], int currentWeight);

stack_data parallelBranchAndBound(stack_data knowResult, knapsack_global params) {
    Item start = {0, 0};

    stack_data best;
    best.state = knowResult.state;
    InitializeAndCopyArray(params.count, knowResult.taken, &best.taken);

    stack_data initProblem;
    initProblem.state = start;
    InitializeArray(params.count, &initProblem.taken);

    #pragma omp parallel
    {
        #pragma omp single
        {
            initStackParallel();
            pushParallel(initProblem);
        }

        while (!isEmptyParallel()) {
            stack_data problem;
            int success = popParallel(&problem);
            if (!success) {
                continue;
            }

            for (int i = 0; i < params.count; ++i) {

                if (problem.taken[i])
                    continue;

                int weight = problem.state.weight + params.items[i].weight;
                if (weight > params.weight) {
                    continue;
                }

                stack_data subProblem;
                subProblem.state.weight = weight;
                subProblem.state.price = problem.state.price + params.items[i].price;
                InitializeAndCopyArray(params.count, problem.taken, &subProblem.taken);
                subProblem.taken[i] = 1;

                if (CanNotAddMoreItems(params, subProblem.taken, subProblem.state.weight)) {
                    if (best.state.price < subProblem.state.price) {
                        #pragma omp critical
                        {
                            if (best.state.price < subProblem.state.price) {
                                printf("Better solution found: %d\n", subProblem.state.price);
                                best.state.price = subProblem.state.price;
                                best.state.weight = subProblem.state.weight;
                                CopyArray(params.count, subProblem.taken, best.taken);
                            }
                        }
                    }

                    continue;
                }

                int knapsackBound = geKnapsackBound(params, subProblem);
                if (knapsackBound < best.state.price) {
                    continue;
                }

                pushParallel(subProblem);
            }
        }

        printf("Thread: %d finished working\n", omp_get_thread_num());
    }

    destroyStack();

    return best;
}

static int geKnapsackBound(knapsack_global params, stack_data problem) {
    Item running = problem.state;

    for (int i = 0; i < params.count; ++i) {
        if (problem.taken[i]) {
            continue;
        }

        int weight = running.weight + params.items[i].weight;

        if (weight >= params.weight) {
            float factor = (float) (params.weight - running.weight) / params.items[i].weight;

            running.price += (int) (factor * params.items[i].price);
            break;
        }

        running.weight = weight;
        running.price += params.items[i].price;
    }

    return running.price;
}

static int CanNotAddMoreItems(knapsack_global params, const int taken[], int currentWeight) {
    for (int i = 0; i < params.count; ++i) {
        if (taken[i]) {
            continue;
        }

        if (currentWeight + params.items[i].weight <= params.weight) {
            return 0;
        }
    }

    return 1;
}