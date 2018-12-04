//
// Created by tomas on 18.9.20.
//

#ifndef KNAPSACK_DATASTRUCTURE_H
#define KNAPSACK_DATASTRUCTURE_H

typedef struct {
    int weight;
    int price;
} Item;

typedef struct {
    int weight;
    int price;
    int index;
} ItemWithIndex;

typedef struct {
    int count;
    int weight;
    Item *items;
    int *originalIndexes;
} knapsack_global;

typedef struct {
    Item state;
    int current;
    int *taken;
} stack_data;

typedef struct stack_node stack_node;
struct stack_node
{
    stack_data data;
    stack_node* next;
};

#endif //KNAPSACK_DATASTRUCTURE_H
