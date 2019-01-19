#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/ParallelStack.h"
#include "../utils.h"
#include "../MPI/mpiWrapper.h"
#include "../DataStructure/LinkedQueue.h"


static int geKnapsackBound(knapsack_global params, stack_data problem, int current);

static int NotAllItemsAdded(int itemCount, int currentItem);

stack_data hybridBranchAndBound(stack_data knowResult, knapsack_global params) {
    int size, rank;
    MPI_Wrapper_Init(params.count, &size, &rank);

    stack_data initialProblem;
    initialProblem.price = 0;
    initialProblem.weight = 0;
    initialProblem.current = -1;
    InitializeArray(params.count, &initialProblem.taken);

    //split init problem
    initQueue();
    enQueue(initialProblem);

    while (queueSize() < 10 * omp_get_max_threads() * size) {
        stack_data problem = deQueue();

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
            enQueue(subProblemNotTaken);
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
            printf("Better solution found: %d\n", subProblemTaken.price);
            knowResult.price = subProblemTaken.price;
            knowResult.weight = subProblemTaken.weight;
            CopyArray(params.count, subProblemTaken.taken, knowResult.taken);
        }

        int knapsackBoundTaken = geKnapsackBound(params, subProblemTaken, next);
        if (knowResult.price < knapsackBoundTaken && NotAllItemsAdded(params.count, next)) {
            enQueue(subProblemTaken);
        } else {
            free(subProblemTaken.taken);
        }

        free(problem.taken);
    }

    // create stack
    initStackParallel();

    // distribute
    int count = 0;
    while (!isQueueEmpty()) {
        int left = queueSize();
        stack_data problem = deQueue();

        int destination = left % size;
        if (destination == rank) {
            count++;
            pushParallel(problem);
        }
    }

    printf("I am process %d with %d threads and have stack of size %d\n", rank, omp_get_max_threads(), count);

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
                        MPI_Wrapper_Receive_Bound(&knowResult);

                        if (knowResult.price <= subProblemTaken.price) {
                            printf("Better solution found: %d, rank: %d\n", subProblemTaken.price, rank);
                            knowResult.price = subProblemTaken.price;
                            knowResult.weight = subProblemTaken.weight;
                            CopyArray(params.count, subProblemTaken.taken, knowResult.taken);

                            MPI_Wrapper_Share_Bound(knowResult);
                        }
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
    }

    destroyStack();

    MPI_Wrapper_Synchronize(&knowResult);
    printf("Finished work on process %d\n", rank);

    MPI_Wrapper_Finalize();

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
    Item running = {.weight = problem.weight, .price = problem.price};

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