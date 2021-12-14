
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define POINTS_MAX 1024 
#define FOLDS_MAX 64

typedef struct Point_t
{
    int x;
    int y;
}
Point_t;

typedef struct Fold_t
{
    char type;
    int val;
}
Fold_t;

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }

    Point_t points[POINTS_MAX];
    int points_cnt = 0;

    while (1)
    {
        assert(points_cnt < POINTS_MAX - 1);
        if (fscanf(fd, "%d,%d\n",&points[points_cnt].x, &points[points_cnt].y) != 2)
            break;
        else
            points_cnt++;
    }
    

    for (int i = 0; i < points_cnt; i++)
        printf("Point: (%d,%d)\n", points[i].x, points[i].y);
    
    
    Fold_t folds[FOLDS_MAX];
    int folds_cnt = 0;
    while (1)
    {
        assert(folds_cnt < FOLDS_MAX - 1);
        if (fscanf(fd, "fold along %c=%d\n",&folds[folds_cnt].type, &folds[folds_cnt].val) != 2)
            break;
        else
            folds_cnt++;
    }

    for (int i = 0; i < folds_cnt; i++)
        printf("Folds: %c at %d\n", folds[i].type, folds[i].val);

    for (int f = 0; f < /*folds_cnt*/ 1 ; f++)
    {
        if (folds[f].type == 'x')
        {
            for (int p = 0; p < points_cnt; p++)
            {
                if (points[p].x > folds[f].val)
                    points[p].x = folds[f].val*2-points[p].x;
                // -(p.x-f) + f == 2f-p.x
            }
        }
        else if (folds[f].type == 'y')
        {
            for (int p = 0; p < points_cnt; p++)
            {
                if (points[p].y > folds[f].val) 
                    points[p].y = folds[f].val*2-points[p].y;
                // -(p.y-f) + f == 2f-p.y
            }
        }   
    }    
    // printf("Points after folding\n");
    // for (int i = 0; i < points_cnt; i++)
    //     printf("Point: (%d,%d)\n", points[i].x, points[i].y);

    Point_t point_set[POINTS_MAX];
    int point_set_cnt = 0;
    for (int i  = 0; i < points_cnt; i++)
    {    
        int add = 1;

        for (int j = 0; j < point_set_cnt; j++)
        {
            if (points[i].x == point_set[j].x && points[i].y == point_set[j].y) {
                add = 0; 
                break;
            }
        }
        if (add)
                point_set[point_set_cnt++] = points[i];
    }
 

    printf("Total points after 1 fold: %d\n", point_set_cnt);

    fclose(fd);

 
    return 0;
}
