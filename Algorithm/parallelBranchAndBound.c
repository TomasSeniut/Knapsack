#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/ParallelStack.h"
#include "../utils.h"

static int geKnapsackBound(knapsack_global params, stack_data problem, int current);

static int NotAllItemsAdded(int itemCount, int currentItem);

stack_data parallelBranchAndBound(stack_data knowResult, knapsack_global params) {

    stack_data initProblem;
    initProblem.price = 0;
    initProblem.weight = 0;
    initProblem.current = -1;
    InitializeArray(params.count, &initProblem.taken);

    initStackParallel();
    pushParallel(initProblem);

    #pragma omp parallel
    {
        while (!isEmptyParallel()) {
            stack_data problem;
            int success = popParallel(&problem);
            if (!success) {
                continue;
            }

            int next = ++problem.current;

            // not take
            stack_data subProblemNotTaken;
            subProblemNotTaken.weight = problem.weight;
            subProblemNotTaken.price = problem.price;
            subProblemNotTaken.current = next;
            InitializeAndCopyArray(params.count, problem.taken, &subProblemNotTaken.taken);
            subProblemNotTaken.taken[next] = 0;

            int knapsackBoundNoTaken = geKnapsackBound(params, subProblemNotTaken, next);
            if (knowResult.price < knapsackBoundNoTaken && NotAllItemsAdded(params.count, next)) {
                pushParallel(subProblemNotTaken);
            } else {
                free(subProblemNotTaken.taken);
            }

            // take
            int weight = problem.weight + params.items[next].weight;
            if (weight > params.weight) {
                free(problem.taken);

                continue;
            }

            stack_data subProblemTaken;
            subProblemTaken.weight = weight;
            subProblemTaken.price = problem.price + params.items[next].price;
            subProblemTaken.current = next;
            InitializeAndCopyArray(params.count, problem.taken, &subProblemTaken.taken);
            subProblemTaken.taken[next] = 1;

            if (knowResult.price <= subProblemTaken.price) {
                #pragma omp critical
                {
                    if (knowResult.price <= subProblemTaken.price) {
                        printf("Better solution found: %d\n", subProblemTaken.price);
                        knowResult.price = subProblemTaken.price;
                        knowResult.weight = subProblemTaken.weight;
                        CopyArray(params.count, subProblemTaken.taken, knowResult.taken);
                    }
                }
            }

            int knapsackBoundTaken = geKnapsackBound(params, subProblemTaken, next);
            if (knowResult.price < knapsackBoundTaken && NotAllItemsAdded(params.count, next)) {
                pushParallel(subProblemTaken);
            } else {
                free(subProblemTaken.taken);
            }

            free(problem.taken);
        }

        printf("Thread: %d finished working\n", omp_get_thread_num());
    }

    destroyStack();

    return knowResult;
}

static int NotAllItemsAdded(int itemCount, int currentItem) {
    if (currentItem + 1 <= itemCount) {
        return 1;
    } else {
        return 0;
    }
}

static int geKnapsackBound(knapsack_global params, stack_data problem, int current) {
    Item running = { .price = problem.price, .weight = problem.weight };

    for (int i = 0; i < params.count; ++i) {
        if (i <= current) {
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