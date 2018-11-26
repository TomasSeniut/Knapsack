//
// Created by tomas on 18.9.22.
//

#include <stdlib.h>
#include <stdio.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

static int geKnapsackBound(knapsack_global params, stack_data problem);
static int CanNotAddMoreItems(knapsack_global params, const int taken[], int currentWeight);

stack_data simpleBranchAndBound(stack_data knowResult, knapsack_global params)
{
    Item start = { 0, 0 };

    stack_data best;
    best.state = knowResult.state;
    InitializeAndCopyArray(params.count, knowResult.taken, &best.taken);

    stack_node *stack = NULL;
    init(stack);

    stack_data initProblem;
    initProblem.state = start;
    InitializeArray(params.count, &initProblem.taken);

    stack = push(stack, initProblem);

    while (!isEmpty(stack))
    {
        stack_data problem;
        stack = pop(stack, &problem);

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

            if (CanNotAddMoreItems(params, subProblem.taken, subProblem.state.weight))
            {
                if (best.state.price < subProblem.state.price) {
                    printf("Better solution found: %d\n", subProblem.state.price);
                    best.state.price = subProblem.state.price;
                    best.state.weight = subProblem.state.weight;
                    CopyArray(params.count, subProblem.taken, best.taken);
                }

                continue;
            }

            int knapsackBound = geKnapsackBound(params, subProblem);
            if (knapsackBound < best.state.price) {
                continue;
            }

            stack = push(stack, subProblem);
        }

        free(problem.taken);
    }

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
            float factor = (float)(params.weight - running.weight) / params.items[i].weight;

            running.price += (int)(factor * params.items[i].price);
            break;
        }

        running.weight = weight;
        running.price += params.items[i].price;
    }

    return running.price;
}

static int CanNotAddMoreItems(knapsack_global params, const int taken[], int currentWeight)
{
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