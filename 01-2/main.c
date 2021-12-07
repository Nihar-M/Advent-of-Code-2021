#include <stdio.h>

int main()
{
    char file[] = "input.txt";

    FILE* fd = fopen(file,"r");
    if (fd == NULL) {
        printf("Error occured opening file %s.\n",file );
        return 1;
    }

    int window[3];
    // a b c
    // ^
    // |
    // most recent

    fscanf(fd,"%d ",&window[2]);
    fscanf(fd,"%d ",&window[1]);
    fscanf(fd,"%d ",&window[0]);

    float old_avg = (float)(window[0]+window[1]+window[2])/3;
    float new_avg;
    int sum = 0;

    while (!feof(fd))
    {
        window[2] = window[1];
        window[1] = window[0];
        fscanf(fd,"%d ",&window[0]);
        new_avg = (float)(window[0]+window[1]+window[2])/3;
        if (new_avg > old_avg) sum++;
        old_avg = new_avg;
    }

    fclose(fd);
    printf("Sum = %d", sum);
    return 0;
}