#include <stdio.h>

int main()
{
    char file[] = "input.txt";

    FILE* fd = fopen(file,"r");
    if (fd == NULL) {
        printf("Error occured opening file %s.\n",file );
        return 1;
    }

    int depth = -1;
    if (fscanf(fd,"%d ",&depth) == 0) {
        printf("Error reading first depth in file %s.\n", file);
        return 1;
    }
    int old;
    int sum = 0;

    while (!feof(fd))
    {
        old = depth;
        fscanf(fd,"%d ",&depth);
        if (depth > old) sum++;
    }

    fclose(fd);

    printf("Sum = %d", sum);
    return 0;
}