
#include <stdio.h>
#include <stdlib.h>

#define MAXCRABS 1024

// try to change to merge sort or quick sort
void sort_crabs(int *crabs, int crabs_size)
{
    for (int i = 0; i < crabs_size; i++)
    {
        for (int j = i; j < crabs_size; j++)
        {
            if (crabs[i] > crabs[j])
            {
                int temp = crabs[i];
                crabs[i] = crabs[j];
                crabs[j] = temp;
            }
        }
    }
}

int abs(int x)
{
    return x * ((x < 0) ? -1 : 1);
}

int fuel_usage(int *crabs, int crabs_size, int target)
{
    int fuel = 0;
    for (int i = 0; i < crabs_size; i++)
    {
        fuel += abs(target - crabs[i]);
    }
    return fuel;
}

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }

    int crabs[MAXCRABS];
    int crabs_size = 0;

    while (1)
    {
        fscanf(fd, "%d", &crabs[crabs_size++]);
        if (fgetc(fd) != ',')
            break;
    }

    sort_crabs(crabs, crabs_size);

    int optimal_index;

    if (crabs_size % 2 == 0)
    {
        int lower_median = crabs_size / 2 - 1;
        int upper_median = crabs_size / 2;
        if (fuel_usage(crabs, crabs_size, crabs[upper_median]) < fuel_usage(crabs, crabs_size, crabs[lower_median]))
            optimal_index = upper_median;
        else
            optimal_index = lower_median;
    }
    else
    {
        optimal_index = (crabs_size - 1) / 2; // because zero indexing
    }

    printf("Optimal Position: %d\n", crabs[optimal_index]);
    printf("Fuel usage: %d\n", fuel_usage(crabs, crabs_size, crabs[optimal_index]));

    fclose(fd);

    return 0;
}
