
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXDISPLAYS 1024

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }
    
    char informations[MAXDISPLAYS][10][8];
    memset(informations, 0, MAXDISPLAYS * 10 * 8 * sizeof(char));
    char readouts[MAXDISPLAYS][4][8];
    memset(readouts, 0, MAXDISPLAYS * 4 * 8 * sizeof(char));
    

    int displays = 0;

    while (!feof(fd))
    {
        for (int i = 0; i < 10; i++)
            fscanf(fd,"%s ",informations[displays][i]);    
        assert(fgetc(fd) == '|');
        for (int i = 0; i < 4; i++)
            fscanf(fd,"%s ",readouts[displays][i]);
        displays++;
    }
    
    int unique = 0;

    for (int i = 0; i < displays; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int segments_on =  strlen(readouts[i][j]);
            if (segments_on == 2 ||
                segments_on == 3 ||
                segments_on == 4 ||
                segments_on == 7)
            {
                unique++;
            }
        }
    }
    
    printf("Number of 1,4,7,8's seen: %d\n",unique);

    fclose(fd);
 
    return 0;
}
