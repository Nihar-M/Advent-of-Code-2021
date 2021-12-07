
#include <stdio.h>
#include <stdlib.h>

#define MAXCRABS 1024

int fuel_usage(int *crabs, int crabs_size, int target)
{
    int fuel = 0;
    for (int i = 0; i < crabs_size; i++)
    {
        int distance = abs(target - crabs[i]);
        fuel += ((distance + 1) * distance) / 2;
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

    int optimal_position;

    int sum = 0;
    for (int i = 0; i < crabs_size; i++)
        sum += crabs[i];
    int average = sum / crabs_size;

    printf("Average position: %d\n", average);

    if (fuel_usage(crabs, crabs_size, average) < fuel_usage(crabs, crabs_size, average + 1))
        optimal_position = average;
    else
        optimal_position = average + 1;

    printf("Optimal Position: %d\n", optimal_position);
    printf("Fuel usage: %d\n", fuel_usage(crabs, crabs_size, optimal_position));

    fclose(fd);

    return 0;
}
