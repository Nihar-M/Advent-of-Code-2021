
#include <stdio.h>

#define ROWS 1000
#define COLS 12

int main()
{
    char file[] = "input.txt";

    FILE* fd = fopen(file,"r");
    if (fd == NULL) {
        printf("Error occured opening file %s.\n",file );
        return 1;
    }

    char data[ROWS][COLS];

    char o2_filter[ROWS];
    int o2_remaining = ROWS;
    char co2_filter[ROWS];
    int co2_remaining = ROWS;

    for (int j = 0; j < ROWS && !feof(fd); j++)
    {

        for(int i = 0; i < COLS; i++)
            data[j][i] = fgetc(fd);
        fgetc(fd); // newline

        printf("Data: %.12s\n", data[j]);
        o2_filter[j] = 1;
        co2_filter[j] = 1;
    }

    for (int j = 0; j < COLS; j++)
    {
        char o2_popular = 0;
        char co2_popular = 0;


        for (int i = 0; i < ROWS; i++)
        {
            if (o2_filter[i]) o2_popular += (data[i][j] == '1') ? 1 : -1;
            if (co2_filter[i]) co2_popular += (data[i][j] == '1') ? 1 : -1;
        }

        for (int i = 0; i < ROWS && o2_remaining > 1; i++)
        {
            if (o2_popular >= 0)
            {
                if (o2_filter[i])
                {
                    if (data[i][j] == '0'){
                        o2_filter[i] = 0;
                        o2_remaining--;
                    }
                }
            }
            else
            {
                if (o2_filter[i])
                {
                    if (data[i][j] == '1'){
                        o2_filter[i] = 0;
                        o2_remaining--;
                    }
                }
            }
        }

        for (int i = 0; i < ROWS && co2_remaining > 1; i++)
        {
            if (co2_popular >= 0)
            {
                if (co2_filter[i])
                {
                    if (data[i][j] == '1'){
                        co2_filter[i] = 0;
                        co2_remaining--;
                    }
                }
            }
            else
            {
                if (co2_filter[i])
                {
                    if (data[i][j] == '0'){
                        co2_filter[i] = 0;
                        co2_remaining--;
                    }
                }
            }
        }
    }

    int o2_final_index = 0;
    for (; o2_final_index < ROWS && !(o2_filter[o2_final_index] == 1); o2_final_index ++){}
    int o2_value = 0;
    for (int i = 0; i < COLS; i++)
    {
        o2_value <<= 1;
        o2_value += (data[o2_final_index][i] == '1');
    }

    int co2_final_index = 0;
    for (; co2_final_index < ROWS && !(co2_filter[co2_final_index] == 1); co2_final_index ++){}
    int co2_value = 0;
    for (int i = 0; i < COLS; i++)
    {
        co2_value <<= 1;
        co2_value += (data[co2_final_index][i] == '1');
    }

    printf("O2  Rate: %d\n", o2_value);
    printf("CO2 Rate: %d\n", co2_value);
    printf("Life support Rating: %d\n", o2_value*co2_value);
    fclose(fd);
    return 0;
}
