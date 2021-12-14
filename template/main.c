
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }

    fclose(fd);
 
    return 0;
}
