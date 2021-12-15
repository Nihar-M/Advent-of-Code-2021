
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define GRID_SIZE 100
#define FRINGE_MAX 100000

typedef struct Risk_t
{
    int risk;
    int visited;
} Risk_t;

void find_lowest_risk(Risk_t **risks, int *lowest_i, int *lowest_j)
{
    *lowest_i = -1;
    *lowest_j = -1;
    // Risk_t *lowest = NULL;
    for (int i = 0; i < GRID_SIZE*5; i++)
    {
        for (int j = 0; j < GRID_SIZE*5; j++)
        {
            if (risks[i][j].risk == -1) continue;
            if (risks[i][j].visited) continue;
            if (*lowest_i == -1) {
                *lowest_i = i;
                *lowest_j = j;
                // lowest = &risks[i][j];
            }
            else {
                if (risks[*lowest_i][*lowest_j].risk > risks[i][j].risk) {
                    *lowest_i = i;
                    *lowest_j = j;
                    // lowest = &risks[i][j];
                }
            }
        }
    }
    if (*lowest_i == -1)
        printf("Failed to find lowest risk level cell.\n");
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

    char grid[GRID_SIZE*5][GRID_SIZE*5];
    
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            int target = fgetc(fd) - '1';
            for (int di = 0; di < 5; di++) 
                for (int dj = 0; dj < 5; dj++)
                    grid[i + di*GRID_SIZE][j + dj*GRID_SIZE] = (target + di + dj)%9 + '1';
            grid[i][j] = target + '1';
        }
        fgetc(fd);
    }

    // for (int i = 0; i < GRID_SIZE*5; i++)
    //     for (int j = 0; j < GRID_SIZE*5; j++)
    //         printf("%c%c",grid[i][j], (j==GRID_SIZE*5-1)?'\n':0);

    Risk_t **risks = malloc(GRID_SIZE*5 * sizeof(Risk_t*));
    for (int i = 0; i < GRID_SIZE*5; i++)
    {
        risks[i] = malloc(GRID_SIZE*5 * sizeof(Risk_t));
        for (int j = 0; j < GRID_SIZE*5; j++)
            risks[i][j] = (Risk_t){-1,0};
    }
    risks[0][0] = (Risk_t){0,0};
    
    while (1)
    {
        int head_i;
        int head_j;
        find_lowest_risk(risks, &head_i, &head_j);

        printf("Checking index (%d,%d) with (%d)|%d|\n", head_i, head_j, risks[head_i][head_j].risk, risks[head_i][head_j].visited);
        risks[head_i][head_j].visited = 1;

        int next_i;
        int next_j;
        if (head_i > 0) {
            next_i = head_i-1;
            next_j = head_j;
            if (risks[next_i][next_j].risk == -1) {
                risks[next_i][next_j].risk = risks[head_i][head_j].risk + (grid[next_i][next_j]-'0');
            }
            else {
                if (risks[head_i][head_j].risk + (grid[next_i][next_j]-'0') < risks[next_i][next_j].risk) {
                    risks[next_i][next_j].risk = risks[head_i][head_j].risk + (grid[next_i][next_j]-'0');
                    risks[next_i][next_j].visited = 0;
                }
            }
        }

        if (head_j > 0) {
            next_i = head_i;
            next_j = head_j-1;
            if (risks[next_i][next_j].risk == -1) {
                risks[next_i][next_j].risk = risks[head_i][head_j].risk + (grid[next_i][next_j]-'0');
            }
            else {
                if (risks[head_i][head_j].risk + (grid[next_i][next_j]-'0') < risks[next_i][next_j].risk) {
                    risks[next_i][next_j].risk = risks[head_i][head_j].risk + (grid[next_i][next_j]-'0');
                    risks[next_i][next_j].visited = 0;
                }
            }
        }   

        if (head_i < -1 + GRID_SIZE*5) {
            next_i = head_i+1;
            next_j = head_j;
            if (risks[next_i][next_j].risk == -1) {
                risks[next_i][next_j].risk = risks[head_i][head_j].risk + (grid[next_i][next_j]-'0');
            }
            else {
                if (risks[head_i][head_j].risk + (grid[next_i][next_j]-'0') < risks[next_i][next_j].risk) {
                    risks[next_i][next_j].risk = risks[head_i][head_j].risk + (grid[next_i][next_j]-'0');
                    risks[next_i][next_j].visited = 0;
                }
            }
        }   

        if (head_j < -1 + GRID_SIZE*5) {
            next_i = head_i;
            next_j = head_j+1;
            if (risks[next_i][next_j].risk == -1) {
                risks[next_i][next_j].risk = risks[head_i][head_j].risk + (grid[next_i][next_j]-'0');
            }
            else {
                if (risks[head_i][head_j].risk + (grid[next_i][next_j]-'0') < risks[next_i][next_j].risk) {
                    risks[next_i][next_j].risk = risks[head_i][head_j].risk + (grid[next_i][next_j]-'0');
                    risks[next_i][next_j].visited = 0;
                }
            }
        }

        if (risks[GRID_SIZE*5-1][GRID_SIZE*5-1].risk >= 0)
            break;
    }

    printf("Final Risk: %d\n", risks[GRID_SIZE*5-1][GRID_SIZE*5-1].risk);

    fclose(fd); 

    return 0;
}
