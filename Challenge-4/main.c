#include <stdio.h>
#include <string.h>

int main()
{
    char file[] = "input.txt";

    FILE* fd = fopen(file,"r");
    if (fd == NULL) {
        printf("Error occured opening file %s.\n",file );
        return 1;
    }

    int depth = 0;
    int distance = 0;
    int aim = 0;

    int size = 0;
    char direction[8];

    while (!feof(fd))
    {
        if (fscanf(fd,"%s %d ",direction, &size) == 2)
        {
            if (strcmp(direction,"forward") == 0)
            {
                distance += size;
                depth += aim*size;
            }
            else if (strcmp(direction,"up") == 0)
                aim -= size;
            else if (strcmp(direction,"down") == 0)
                aim += size;
            else
                printf("Direction not recognised");
        }
    }

    fclose(fd);
    printf("Distance = %d\n", distance);
    printf("Depth = %d\n", depth);
    return 0;
}