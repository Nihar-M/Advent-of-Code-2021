
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define STEP_COUNT 100

int has_flashers(int grid[][10])
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (grid[i][j] > 9)
                return 1;
    return 0;
}

void increment(int grid[][10], int i, int j)
{
    if (i >= 0 && i < 10 && j >= 0 && j < 10) 
        if (grid[i][j] != 0)
            grid[i][j]++;
}

int flash(int grid[][10], int i, int j)
{
    if (grid[i][j] <= 9) return 0;

    // printf("flashing (%d,%d)\n", i,j); 
    grid[i][j] = 0;
    
    for (int di = -1; di <= 1; di++)
        for (int dj = -1; dj <= 1; dj++)
            increment(grid, i + di, j + dj);
    return 1;
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

    int grid[10][10];
    {
    char buffer[12];
    for (int i = 0; i < 10; i++)
    {
        fgets(buffer, 12, fd);
        for (int j = 0; j < 10; j++)
            grid[i][j] = buffer[j] - '0';
    }
    }

    // for (int i = 0; i < 10; i++)
    // {
    //     for (int j = 0; j < 10; j++)
    //         printf("%d",grid[i][j]);
    //     printf("\n");
    // }
    
    int total_flashes = 0;

    for (int step = 0; step < STEP_COUNT; step++)
    {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                grid[i][j]++;
        
        
        // 1-9 normal steps
        // 10 -> wants to flash
        // 0 -> has flashed
        while (has_flashers(grid))
        {
            for (int i = 0; i < 10; i++)
                for (int j = 0; j < 10; j++)
                    total_flashes += flash(grid,i,j); 
        }

        // for (int i = 0; i < 10; i++)
        // {
        //     for (int j = 0; j < 10; j++)
        //         printf("%d",grid[i][j]);
        //     printf("\n");
        // }        
    }

    printf("Total Flashes: %d\n", total_flashes);

    fclose(fd);
 
    return 0;
}
