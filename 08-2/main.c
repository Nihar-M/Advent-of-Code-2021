
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXDISPLAYS 1024

int char_in(char *str, char target)
{
    for (int i = 0; i < 7; i++)
    {
        if (str[i] == target)
            return 1;
    }
    return 0;
}

int get_number(char* abcdefg_coding, char* number)
{
    switch (strlen(number))
    {
    case 2:
        return 1;
    case 3:
        return 7;
    case 4:
        return 4;
    case 7:
        return 8;
    case 5:
        if (char_in(number, abcdefg_coding['e'-'a'])) 
            return 2;
        if (char_in(number, abcdefg_coding['b'-'a'])) 
            return 5;
        return 3;
    case 6:
        if (char_in(number, abcdefg_coding['d'-'a']))
        {
            if (char_in(number, abcdefg_coding['c'-'a']))
                return 9;
            else
                return 6;
        }
        else
            return 0;
    }

    assert(0 && "bad number");
    return -1;
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

    char informations[MAXDISPLAYS][10][8];
    memset(informations, 0, MAXDISPLAYS * 10 * 8 * sizeof(char));
    char readouts[MAXDISPLAYS][4][8];
    memset(readouts, 0, MAXDISPLAYS * 4 * 8 * sizeof(char));

    int displays = 0;

    while (!feof(fd))
    {
        for (int i = 0; i < 10; i++)
            fscanf(fd, "%s ", informations[displays][i]);
        assert(fgetc(fd) == '|');
        for (int i = 0; i < 4; i++)
            fscanf(fd, "%s ", readouts[displays][i]);
        displays++;
    }

    int final_sum = 0;

    for (int i = 0; i < displays; i++)
    {

        char abcdefg[7] = {0, 0, 0, 0, 0, 0, 0};

        int freq[7] = {0, 0, 0, 0, 0, 0, 0};

        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                freq[k] += char_in(informations[i][j], 'a' + k);
            }
        }

        for (int j = 0; j < 7; j++)
        {
            if (freq[j] == 4)
                abcdefg['e' - 'a'] = 'a' + j;
            if (freq[j] == 6)
                abcdefg['b' - 'a'] = 'a' + j;
            if (freq[j] == 9)
                abcdefg['f' - 'a'] = 'a' + j;
        }

        {
            int one_index;
            for (int j = 0; j < 10; j++)
            {
                if (strlen(informations[i][j]) == 2)
                    one_index = j;
            }
            if (informations[i][one_index][0] == abcdefg['f' - 'a'])
                abcdefg['c' - 'a'] = informations[i][one_index][1];
            else
                abcdefg['c' - 'a'] = informations[i][one_index][0];
        }

        {
            int seven_index;
            for (int j = 0; j < 10; j++)
            {
                if (strlen(informations[i][j]) == 3)
                    seven_index = j;
            }
            for (int k = 0; k < 3; k++)
            {
                if (informations[i][seven_index][k] != abcdefg['f' - 'a'] &&
                    informations[i][seven_index][k] != abcdefg['c' - 'a'])
                    abcdefg['a' - 'a'] = informations[i][seven_index][k];
            }
        }

        {
            int four_index;
            for (int j = 0; j < 10; j++)
            {
                if (strlen(informations[i][j]) == 4)
                    four_index = j;
            }
            for (int k = 0; k < 4; k++)
            {
                if (informations[i][four_index][k] != abcdefg['f' - 'a'] &&
                    informations[i][four_index][k] != abcdefg['c' - 'a'] &&
                    informations[i][four_index][k] != abcdefg['b' - 'a'])
                    abcdefg['d' - 'a'] = informations[i][four_index][k];
            }
        }

        {
            char all_abcdefg[7] = "abcdefg";
            for (int j = 0; j < 7; j++)
            {
                if (!char_in(abcdefg, all_abcdefg[j]))
                    abcdefg['g' - 'a'] = all_abcdefg[j];
            }
        }

        // printf("Encoding: %.7s\n", abcdefg);

        int final_readout = 0;
        for (int j = 0; j < 4; j++)
        {
            final_readout *= 10;
            final_readout += get_number(abcdefg, readouts[i][j]);
        }
        // printf("Number: %d\n", final_readout);
        final_sum += final_readout;
    }

    // e appears 4 times
    // b appears 6 times
    // f appears 9 times

    // c from 1
    // a from 7
    // d from 4

    // g is last one

    printf("Output sum: %d\n", final_sum);

    fclose(fd);

    return 0;
}
