//
// Created by tomas on 18.9.22.
//

#include <stdlib.h>
#include <stdio.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

static int geKnapsackBound(knapsack_global params, stack_data problem, int current);

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
    initProblem.current = -1;
    InitializeArray(params.count, &initProblem.taken);

    stack = push(stack, initProblem);

    while (!isEmpty(stack))
    {
        stack_data problem;
        stack = pop(stack, &problem);

        int current = ++problem.current;

        // not take
        stack_data subProblemNotTaken;
        subProblemNotTaken.state.weight = problem.state.weight;
        subProblemNotTaken.state.price = problem.state.price;
        subProblemNotTaken.current = current;
        InitializeAndCopyArray(params.count, problem.taken, &subProblemNotTaken.taken);
        subProblemNotTaken.taken[current] = 0;

        if (best.state.price < subProblemNotTaken.state.price) {
            printf("Better solution found: %d\n", subProblemNotTaken.state.price);
            best.state.price = subProblemNotTaken.state.price;
            best.state.weight = subProblemNotTaken.state.weight;
            CopyArray(params.count, subProblemNotTaken.taken, best.taken);
        }

        int knapsackBoundNoTaken = geKnapsackBound(params, subProblemNotTaken, current);
        if (best.state.price < knapsackBoundNoTaken && current <= params.count - 1) {
            stack = push(stack, subProblemNotTaken);
        }

        // take
        int weight = problem.state.weight + params.items[current].weight;
        if (weight > params.weight) {
            continue;
        }

        stack_data subProblemTaken;
        subProblemTaken.state.weight = weight;
        subProblemTaken.state.price = problem.state.price + params.items[current].price;
        subProblemTaken.current = current;
        InitializeAndCopyArray(params.count, problem.taken, &subProblemTaken.taken);
        subProblemTaken.taken[current] = 1;

        if (best.state.price < subProblemTaken.state.price) {
            printf("Better solution found: %d\n", subProblemTaken.state.price);
            best.state.price = subProblemTaken.state.price;
            best.state.weight = subProblemTaken.state.weight;
            CopyArray(params.count, subProblemTaken.taken, best.taken);
        }

        int knapsackBoundTaken = geKnapsackBound(params, subProblemTaken, current);
        if (best.state.price < knapsackBoundTaken && current <= params.count - 1) {
            stack = push(stack, subProblemTaken);
        }

        free(problem.taken);
    }

    return best;
}

static int geKnapsackBound(knapsack_global params, stack_data problem, int current) {
    Item running = problem.state;

    for (int i = 0; i < params.count; ++i) {
        if (i <= current) {
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