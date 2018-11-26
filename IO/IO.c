//
// Created by tomas on 18.9.20.
//

#include <stdio.h>
#include <stdlib.h>
#include "../DataStructure/DataStructure.h"
#include "../utils.h"

void ReadItemsData(FILE *fp, ItemWithIndex items[]);

knapsack_global ReadGlobalData(const char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r");

    int weight, count;

    fscanf(fp, "# Items: %d, Weight: %d\n", &count, &weight);

    knapsack_global params;
    params.count = count;
    params.weight = weight;

    ItemWithIndex items_index[params.count];
    ReadItemsData(fp, items_index);

    fclose(fp);

    qsort(items_index, (size_t) params.count, sizeof(ItemWithIndex), itemIndexComparePriceWeightRatio);

    params.items = (Item*)malloc(sizeof(Item) * params.count);
    params.originalIndexes = (int*)malloc(sizeof(int) * params.count);

    CopyItemsArray(params.count, items_index, params.items, params.originalIndexes);

    return params;
}

void ReadItemsData(FILE *fp, ItemWithIndex items[])
{
    char buffer[255];

    int i = 0;
    while (!feof (fp)) {
        fgets(buffer, 255, fp);

        if (buffer[0] == '#')
            continue;

        sscanf(buffer, "%d %d %d\n", &(items[i].index), &(items[i].weight), &(items[i].price));
        i++;
    }

    fclose(fp);
}


void PrintResult(knapsack_global params, stack_data solution)
{
    printf("Knapsack price: %d, weight: %d\n", solution.state.price, solution.state.weight);
    printf("Taken items:");
    for (int i = 0; i < params.count; ++i) {
        int index = params.originalIndexes[i];
        printf(" %d:%d", index, solution.taken[i]);
    }
    printf("\n");
}
