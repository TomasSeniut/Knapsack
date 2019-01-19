#include <stdlib.h>
#include <stdio.h>
#include "../DataStructure/DataStructure.h"
#include "../DataStructure/LinkedStack.h"
#include "../utils.h"

static int geKnapsackBound(knapsack_global params, stack_data problem, int current);

stack_data simpleBranchAndBound(stack_data knowResult, knapsack_global params)
{
    stack_node *stack = NULL;
    init(stack);

    stack_data initProblem;
    initProblem.weight = 0;
    initProblem.price = 0;
    initProblem.current = -1;
    InitializeArray(params.count, &initProblem.taken);

    stack = push(stack, initProblem);

    while (!isEmpty(stack))
    {
        stack_data problem;
        stack = pop(stack, &problem);

        int next = ++problem.current;

        // not take
        stack_data subProblemNotTaken;
        subProblemNotTaken.weight = problem.weight;
        subProblemNotTaken.price = problem.price;
        subProblemNotTaken.current = next;
        InitializeAndCopyArray(params.count, problem.taken, &subProblemNotTaken.taken);
        subProblemNotTaken.taken[next] = 0;

        int knapsackBoundNoTaken = geKnapsackBound(params, subProblemNotTaken, next);
        if (knowResult.price < knapsackBoundNoTaken && next <= params.count - 1) {
            stack = push(stack, subProblemNotTaken);
        }

        // take
        int weight = problem.weight + params.items[next].weight;
        if (weight > params.weight) {
            continue;
        }

        stack_data subProblemTaken;
        subProblemTaken.weight = weight;
        subProblemTaken.price = problem.price + params.items[next].price;
        subProblemTaken.current = next;
        InitializeAndCopyArray(params.count, problem.taken, &subProblemTaken.taken);
        subProblemTaken.taken[next] = 1;

        if (knowResult.price < subProblemTaken.price) {
            printf("Better solution found: %d\n", subProblemTaken.price);
            knowResult.price = subProblemTaken.price;
            knowResult.weight = subProblemTaken.weight;
            CopyArray(params.count, subProblemTaken.taken, knowResult.taken);
        }

        int knapsackBoundTaken = geKnapsackBound(params, subProblemTaken, next);
        if (knowResult.price < knapsackBoundTaken && next <= params.count - 1) {
            stack = push(stack, subProblemTaken);
        }

        free(problem.taken);
    }

    return knowResult;
}

static int geKnapsackBound(knapsack_global params, stack_data problem, int current) {
    Item running = { .weight = problem.weight, .price = problem.price};


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