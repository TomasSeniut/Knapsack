//
// Created by tomas on 18.9.22.
//

#include <stddef.h>
#include <stdlib.h>
#include <zconf.h>
#include <stdio.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

int geKnapsackBound(knapsack_global params, Item *items, stack_data problem);

stack_data simpleBranchAndBound(stack_data knowResult, knapsack_global params, Item *items)
{
    Item start = { 0, 0 };

    stack_data best;
    best.state = knowResult.state;
    InitializeAndCopyArray(params.items, knowResult.taken, &best.taken);

    stack_node *stack = NULL;
    init(stack);

    stack_data initProblem;
    initProblem.state = start;
    InitializeArray(params.items, &initProblem.taken);

    stack = push(stack, initProblem);

    while (!isEmpty(stack))
    {
        stack_data problem;
        stack = pop(stack, &problem);

        for (int i = 0; i < params.items; ++i) {

            if (problem.taken[i] == 1)
                continue;

            //Check if leaf
            int weight = problem.state.weight + items[i].weight;
            if (weight >= params.weight)
            {
                if (weight == params.weight)
                {
                    problem.state.weight = weight;
                    problem.state.price += items[i].price;
                    problem.taken[i] = 1;
                }

                if (best.state.price < problem.state.price) {
                    best.state.price = problem.state.price;
                    best.state.weight = problem.state.weight;
                    CopyArray(params.items, problem.taken, best.taken);
                }

                continue;
            }

            int knapsackBound = geKnapsackBound(params, items, problem);
            if (knapsackBound < best.state.price) {
                continue;
            }

            stack_data subProblem;
            subProblem.state.weight = weight;
            subProblem.state.price = problem.state.price + items[i].price;
            InitializeAndCopyArray(params.items, problem.taken, &subProblem.taken);
            subProblem.taken[i] = 1;

            stack = push(stack, subProblem);
        }

        free(problem.taken);
    }

    return best;
}

int geKnapsackBound(knapsack_global params, Item *items, stack_data problem) {
    Item running = problem.state;

    for (int i = 0; i < params.items; ++i) {
        if (problem.taken[i] == 1) {
            continue;
        }

        if (running.weight + items[i].weight >= params.weight) {
            float factor = (float)(params.weight - running.weight) / items[i].weight;

            running.price += (int)(factor * items[i].price);
            break;
        }

        running.price += items[i].price;
        running.weight += items[i].weight;
    }

    return running.price;
}