
#include <stdio.h>

#define NUMBER_CAP 1024
#define BOARD_CAP 1024

void reveal_number(int board[][5], int revealed_number)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == revealed_number)
                board[i][j] = -1;
}

int check_solved(int board[][5])
{
    for (int i = 0; i < 5; i++)
    {
        int won = 1;
        for (int j = 0; j < 5; j++)
            if (board[i][j] != -1)
                won = 0;
        if (won)
            return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        int won = 1;
        for (int j = 0; j < 5; j++)
            if (board[j][i] != -1)
                won = 0;
        if (won)
            return 1;
    }

    return 0;
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

    int revealed[NUMBER_CAP];
    int revealed_size = 0;

    while (1)
    {
        fscanf(fd, "%d", &revealed[revealed_size]);
        revealed_size++;
        if (fgetc(fd) != ',')
            break;
    }

    int boards[BOARD_CAP][5][5];
    int boards_size = 0;

    for (; fgetc(fd) == '\n'; boards_size++)
    {
        for (int i = 0; i < 5; i++)
        {
            fscanf(fd, "%d %d %d %d %d", boards[boards_size][i] + 0, boards[boards_size][i] + 1, boards[boards_size][i] + 2, boards[boards_size][i] + 3, boards[boards_size][i] + 4);
            fgetc(fd);
        }
    }

    int boards_won[BOARD_CAP];

    for (int i = 0; i < BOARD_CAP; i++)
        boards_won[i] = 0;

    int boards_won_remaining = boards_size;

    int final_number_index;
    int final_board_index;

    for (int i = 0; i < revealed_size; i++)
    {
        for (int j = 0; j < boards_size; j++)
        {

            if (!boards_won[j])
            {
                reveal_number(boards[j], revealed[i]);

                if (check_solved(boards[j]))
                {
                    boards_won[j] = 1;

                    boards_won_remaining--;
                    if (boards_won_remaining == 0)
                    {
                        final_board_index = j;
                        break;
                    }
                }
            }
        }

        if (boards_won_remaining == 0)
        {
            final_number_index = i;
            break;
        }
    }

    printf("Final number: %d\n", revealed[final_number_index]);

    int final_sum = 0;
    for (int i_ = 0; i_ < 5; i_++)
    {
        for (int j_ = 0; j_ < 5; j_++)
        {
            final_sum += boards[final_board_index][i_][j_];
            if (boards[final_board_index][i_][j_] == -1)
                final_sum++;
            printf("%d%s", boards[final_board_index][i_][j_], (j_ == 4) ? "\n" : " ");
        }
    }

    printf("Final Board sum: %d\n", final_sum);
    printf("Final Score: %d\n", final_sum * revealed[final_number_index]);

    fclose(fd);
    return 0;
}
