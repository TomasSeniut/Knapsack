//
// Created by tomas on 18.12.4.
//

#ifndef KNAPSACK_LINKEDSTACK_H
#define KNAPSACK_LINKEDSTACK_H

#include "DataStructure.h"

int isEmpty(stack_node *s);
stack_node* push(stack_node *s, stack_data data);
stack_node* pop(stack_node *s, stack_data *data);
void init(stack_node* s);

#endif //KNAPSACK_LINKEDSTACK_H
