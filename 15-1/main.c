
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define GRID_SIZE 100

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }

    char grid[GRID_SIZE][GRID_SIZE];
    
    for (int i = 0; i < GRID_SIZE; i++)
    {
        fscanf(fd,"%s",grid[i]);
    }

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            printf("%c%c",grid[i][j], (j==GRID_SIZE-1)?'\n':0);
    
    int diagonal[GRID_SIZE];
    int diagonal_old[GRID_SIZE];
    diagonal[0] = 0;
    for (int sl = 1; sl < GRID_SIZE; sl++)
    {
        for (int i = 0; i < GRID_SIZE; i++)
            diagonal_old[i] = diagonal[i];
        diagonal[0] = diagonal_old[0] + grid[sl][0]-'0';
        for (int i = 1; i < sl /*-1*/; i++)
        {
            // printf("(%d)", grid[sl-1-i][i]-'0');
            if (diagonal_old[i-1] < diagonal_old[i])
                diagonal[i] = diagonal_old[i-1] + grid[sl-i][i]-'0'; 
            else
                diagonal[i] = diagonal_old[i] + grid[sl-i][i]-'0'; 
        }
        diagonal[sl] = diagonal_old[sl-1] + grid[0][sl]-'0';
        for (int i = 0; i < sl+1; i++)
            printf("%d,",diagonal[i]);
        printf("\n");    
    }    

    for (int sl = GRID_SIZE-1; sl >= 0; sl--)
    {
        for (int i = 0; i < GRID_SIZE; i++)
            diagonal_old[i] = diagonal[i];

        for (int i = 0; i < sl; i++)
        {
            // printf("%d vs %d\n", diagonal_old[i], diagonal_old[i+1]);
            // printf("(%d,%d),", GRID_SIZE-i-1,(GRID_SIZE-sl)+i);
            // printf("%c,",grid[GRID_SIZE-i-1][(GRID_SIZE-sl)+i]);
            if (diagonal_old[i] < diagonal_old[i+1])
                diagonal[i] = diagonal_old[i] + grid[GRID_SIZE-i-1][GRID_SIZE-sl+i]-'0';
            else 
                diagonal[i] = diagonal_old[i+1] + grid[GRID_SIZE-i-1][GRID_SIZE-sl+i]-'0';
        }

        printf("\n");
        for (int i = 0; i < sl; i++)
            printf("%d,",diagonal[i]);
        printf("\n");    
    }    
    

    fclose(fd); 

    return 0;
}
