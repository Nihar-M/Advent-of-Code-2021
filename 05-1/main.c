
#include <stdio.h>
#include <stdlib.h>

#define MAPSIZE 1000
#define MAXLINES 1024

typedef struct Line_t
{
    int sx;
    int sy;
    int ex;
    int ey;
} Line_t;

void add_line(int **map, Line_t line)
{
    if (line.sx == line.ex)
    {
        int start = (line.sy < line.ey) ? line.sy : line.ey;
        int end = (line.sy > line.ey) ? line.sy : line.ey;
        for (int i = start; i <= end; i++)
        {
            map[i][line.sx] += 1;
        }
    }
    else if (line.sy == line.ey)
    {
        int start = (line.sx < line.ex) ? line.sx : line.ex;
        int end = (line.sx > line.ex) ? line.sx : line.ex;
        for (int i = start; i <= end; i++)
        {
            map[line.sy][i] += 1;
        }
    }
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

    Line_t lines[MAXLINES];
    int lines_size = 0;
    while (1)
    {
        fscanf(fd, "%d,%d -> %d,%d", &lines[lines_size].sx, &lines[lines_size].sy, &lines[lines_size].ex, &lines[lines_size].ey);
        if (fgetc(fd) != '\n')
            break;
        lines_size++;
    }

    for (int i = 0; i < lines_size; i++)
    {
        if (lines[i].sx < 0 || lines[i].sx >= MAPSIZE ||
            lines[i].sy < 0 || lines[i].sy >= MAPSIZE ||
            lines[i].ex < 0 || lines[i].ex >= MAPSIZE ||
            lines[i].ey < 0 || lines[i].ey >= MAPSIZE)
        {
            printf("MAPSIZE too small.\n");
        }
        // printf("Line: %d,%d -> %d,%d\n", lines[i].sx, lines[i].sy, lines[i].ex, lines[i].ey);
    }

    printf("Number of lines: %d\n", lines_size);

    int **map = malloc(MAPSIZE * sizeof(void *));

    for (int i = 0; i < MAPSIZE; i++)
        map[i] = malloc(MAPSIZE * sizeof(int));

    for (int i = 0; i < MAPSIZE; i++)
        for (int j = 0; j < MAPSIZE; j++)
            map[i][j] = 0;

    for (int i = 0; i < lines_size; i++)
    {
        add_line((int **)map, lines[i]);
    }

    // for (int i = 0; i < MAPSIZE; i++)
    //     for (int j = 0; j < MAPSIZE; j++)
    //         printf("%d%c", map[i][j], (j==(MAPSIZE-1)) ? '\n' : ' ');

    int dangers = 0;
    for (int i = 0; i < MAPSIZE; i++)
        for (int j = 0; j < MAPSIZE; j++)
            if (map[i][j] >= 2)
                dangers++;

    printf("Total Dangers: %d", dangers);

    for (int i = 0; i < MAPSIZE; i++)
        free(map[i]);
    free(map);

    fclose(fd);

    return 0;
}
