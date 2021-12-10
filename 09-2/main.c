
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define MAX_COL 256
#define MAX_ROW 1024

#define MAX_LOWPOINT 1024
// not really sure how large this needs to be

int in_array(int i, int j, int arr[][2], int arr_size)
{
    for (int x = 0; x < arr_size; x++)
        if (arr[x][0] == i && arr[x][1] == j)
            return 1;
    return 0;
}

int flood_fill(int* start, char terrain_map[][MAX_COL], int lines, int line_size)
{
    int visited[1024][2];
    int visited_size = 0;
    int fringe[1024][2];
    int fringe_size = 1;
    fringe[0][0] = start[0];
    fringe[0][1] = start[1];
    while (fringe_size > 0)
    {
        if (fringe_size >= 1024-2)
            assert(0 && "Fringe CAP exceeded.");
        if (visited_size >= 1024-2)
            assert(0 && "Visited CAP exceeded.");
        int i = fringe[fringe_size-1][0];
        int j = fringe[fringe_size-1][1];
        visited[visited_size][0] = i;
        visited[visited_size][1] = j;
        visited_size++;
        fringe_size--;
        // printf("Fringe: (%d,%d)  %d\t Visited: %d\n",fringe[0][0],fringe[0][1], fringe_size,visited_size);       

        if (i > 0) 
            if (terrain_map[i][j] <= terrain_map[i-1][j] && terrain_map[i-1][j] != '9' && !in_array(i-1,j,visited,visited_size) && !in_array(i-1,j,fringe,fringe_size)){
                fringe[fringe_size][0] = i-1;
                fringe[fringe_size][1] = j;
                fringe_size++;
            }
        if (i < lines - 1)
            if (terrain_map[i][j] <= terrain_map[i+1][j] && terrain_map[i+1][j] != '9' && !in_array(i+1,j,visited,visited_size) && !in_array(i+1,j,fringe,fringe_size)){
                fringe[fringe_size][0] = i+1;
                fringe[fringe_size][1] = j;
                fringe_size++;
            }
        if (j > 0)
            if (terrain_map[i][j] <= terrain_map[i][j-1] && terrain_map[i][j-1] != '9' && !in_array(i,j-1,visited,visited_size) && !in_array(i,j-1,fringe,fringe_size)){
                fringe[fringe_size][0] = i;
                fringe[fringe_size][1] = j-1;
                fringe_size++; 
            }
        if (j < line_size - 1) 
            if (terrain_map[i][j] <= terrain_map[i][j+1] && terrain_map[i][j+1] != '9' && !in_array(i,j+1,visited,visited_size) && !in_array(i,j+1,fringe,fringe_size)){
                fringe[fringe_size][0] = i;
                fringe[fringe_size][1] = j+1;
                fringe_size++; 
            }
    }
    
    // printf("Was (1,2) visited?? %d\n", in_array(1,2,visited,visited_size));

    // printf("Visited: %d \n", visited_size);
    // for (int i = 0; i < visited_size; i++)
    // {
    //     printf(" (%d,%d)\n",visited[i][0],visited[i][1]);
    // }
    return visited_size;
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
    
    char terrain_map[MAX_ROW][MAX_COL];

    memset(terrain_map, 0, sizeof(terrain_map));

    fgets(terrain_map[0], MAX_COL, fd);
    int line_size = strlen(terrain_map[0]) - 1;
    int lines = 0;

    while(!feof(fd))
    {
        fgets(terrain_map[++lines], MAX_COL, fd);
    }
    


    // for (int i = 0; i < lines; i++)
    //     for (int j = 0; j < line_size; j++)
    //         printf("%c%c",terrain_map[i][j],(j==line_size-1)?'\n':'\0');

    // printf("Lines: %d\n", lines);

    int low_points[MAX_LOWPOINT][2];
    int low_points_size = 0;

    for (int i = 0; i < lines; i++)
    {    
        for (int j = 0; j < line_size; j++)
        {
            if (i > 0) 
                if (terrain_map[i][j] >= terrain_map[i-1][j])
                    continue;
            if (i < lines - 1) 
                if (terrain_map[i][j] >= terrain_map[i+1][j])
                    continue;
            if (j > 0) 
                if (terrain_map[i][j] >= terrain_map[i][j-1])
                    continue;
            if (j < line_size - 1) 
                if (terrain_map[i][j] >= terrain_map[i][j+1])
                    continue;
            // printf("Low point of %d at (%d, %d)\n", terrain_map[i][j],i,j);
            low_points[low_points_size][0] = i;
            low_points[low_points_size][1] = j;
            low_points_size++;
        }
    }
           
    int basin_sizes[MAX_LOWPOINT];
    
    for (int i = 0; i < low_points_size; i++)
    {
        basin_sizes[i] = flood_fill(low_points[i],terrain_map,lines,line_size);
    }
    fclose(fd);
 
    for (int i = 0; i < low_points_size; i++)
    {
        for (int j = 0; j < low_points_size; j++)
        {
            if (basin_sizes[j] < basin_sizes[i])
            {
                int temp = basin_sizes[j];
                basin_sizes[j] = basin_sizes[i];
                basin_sizes[i] = temp;
            }
        }
    }   
    
 
    for (int i = 0; i < 3; i++)
        printf("Basin Size: %d\n", basin_sizes[i]);

    printf("Final product: %d\n", basin_sizes[0] * basin_sizes[1] * basin_sizes[2]);
    

    return 0;
}
