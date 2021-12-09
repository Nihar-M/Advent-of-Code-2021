
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define MAXCOLNUM 256
#define MAXROWNUM 1024

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }
    
    char terrain_map[MAXROWNUM][MAXCOLNUM];

    memset(terrain_map, 0, sizeof(terrain_map));

    fgets(terrain_map[0], MAXCOLNUM, fd);
    int line_size = strlen(terrain_map[0]) - 1;
    int lines = 0;

    while(!feof(fd))
    {
        fgets(terrain_map[++lines], MAXCOLNUM, fd);
    }
    


    // for (int i = 0; i < lines; i++)
    //     for (int j = 0; j < line_size; j++)
    //         printf("%c%c",terrain_map[i][j],(j==line_size-1)?'\n':'\0');

    // printf("Lines: %d\n", lines);

    int risk_level = 0;

    for (int i = 0; i < lines; i++)
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
            risk_level += 1 + (terrain_map[i][j] - '0');
        }
    
    printf("Total Risk Levels: %d\n", risk_level);
   
    fclose(fd);
 
    return 0;
}
