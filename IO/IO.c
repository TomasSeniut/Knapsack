//
// Created by tomas on 18.9.20.
//

#include <stdio.h>
#include "../DataStructure/DataStructure.h"

knapsack_global ReadGlobalData(const char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r");

    int weight, items;

    fscanf(fp, "# Items: %d, Weight: %d", &items, &weight);

    fclose(fp);

    knapsack_global params;
    params.items = items;
    params.weight = weight;
    return params;
}

void ReadItemsData(const char *fileName, ItemWithIntedexes items[])
{
    FILE *fp;
    fp = fopen(fileName, "r");

    char buffer[255];

    int i = 0, a;
    while (!feof (fp)) {
        fgets(buffer, 255, fp);

        if (buffer[0] == '#')
            continue;

        sscanf(buffer, "%d %d %d", &(items[i].index), &(items[i].weight), &(items[i].price));
        i++;
    }

    fclose(fp);
}


void PrintResult(knapsack_global params, stack_data solution, int correctIndexes[])
{
    printf("Solution from B&B:\n Knapsack price: %d, weight: %d\n", solution.state.price, solution.state.weight);
    printf("Taken items:");
    for (int i = 0; i < params.items; ++i) {
        int index = correctIndexes[i];
        printf(" %d:%d", index, solution.taken[i]);
    }
    printf("\n"); // rez: [0, 2, 3, 4, 6, 7, 8, 9, 10, 11] //missing [1, 5
}
