//
// Created by tomas on 18.9.20.
//

#ifndef KNAPSACK_DATASTRUCTURE_H
#define KNAPSACK_DATASTRUCTURE_H

typedef struct {
    int items;
    int weight;
} knapsack_global;

typedef struct {
    int weight;
    int price;
} Item;

typedef struct {
    Item state;
    int *taken;
} stack_data;

typedef struct stack_node stack_node;
struct stack_node
{
    stack_data data;
    stack_node* next;
};

int isEmpty(stack_node *s);
stack_node* push(stack_node *s, stack_data data);
stack_node* pop(stack_node *s, stack_data *data);
void init(stack_node* s);

#endif //KNAPSACK_DATASTRUCTURE_H
