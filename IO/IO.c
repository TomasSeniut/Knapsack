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

void ReadItemsData(const char *fileName, Item items[])
{
    FILE *fp;
    fp = fopen(fileName, "r");

    char buffer[255];

    int i = 0, a;
    while (!feof (fp)) {
        fgets(buffer, 255, fp);

        if (buffer[0] == '#')
            continue;

        sscanf(buffer, "%d %d %d", &a, &(items[i].weight), &(items[i].price));
        i++;
    }

    fclose(fp);
}
