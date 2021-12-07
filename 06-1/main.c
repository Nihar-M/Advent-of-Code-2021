
#include <stdio.h>
#include <stdlib.h>

#define DAYSRUN 80

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }

    size_t populations[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    // 0 - 8

    {
        int age = 0;
        while (1)
        {
            fscanf(fd, "%d", &age);
            populations[age]++;
            if (fgetc(fd) != ',')
                break;
        }
    }

    {
        size_t total = 0;
        printf("Population: ");
        for (int i = 0; i < 9; i++)
        {
            printf("%I64u ", populations[i]);
            total += populations[i];
        }
        printf("\t\tTotal: %I64u\n", total);
    }

    for (int day = 0; day < DAYSRUN; day++)
    {
        size_t births = populations[0];
        for (int i = 1; i < 9; i++)
            populations[i - 1] = populations[i];
        populations[8] = 0;

        populations[6] += births;
        populations[8] += births;
    }

    {
        size_t total = 0;
        printf("Population: ");
        for (int i = 0; i < 9; i++)
        {
            printf("%I64u ", populations[i]);
            total += populations[i];
        }
        printf("\t\tTotal; %I64u\n", total);
    }

    fclose(fd);

    return 0;
}
