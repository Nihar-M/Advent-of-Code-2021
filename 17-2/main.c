
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
    
    int xmin;
    int xmax;
    int ymin;
    int ymax;

    fscanf(fd, "target area: x=%d..%d, y=%d..%d", &xmin, &xmax, &ymin, &ymax);

    // printf("target area: x=%d..%d, y=%d..%d\n", xmin, xmax, ymin, ymax);

    int vxmin = -1;
    int vxmax = xmax;
    
    for (int i = 0; i < 100; i++)
    {
        int tri = (i)*(i+1)/2;
        if (vxmin == -1){
            if (xmin <= tri && tri <= xmax) {
                vxmin = i;
                break;
            }
        }
    }

    // printf("possible vx: %d-%d\n", vxmin, vxmax);

    int total_hits = 0;

    for (int vyi = ymin; vyi < 1000; vyi++)
    {
        for (int vxi = vxmin; vxi <= vxmax; vxi++)
        {
            int x = 0;
            int y = 0;
            int vx = vxi;
            int vy = vyi;
            int hits = 0;
            int max_y = 0;
            while (y >= ymin && x <= xmax)
            {
                if (ymin <= y && y <= ymax && xmin <= x && x <= xmax) {
                    hits = 1;
                    // printf("(%d,%d) hits and gets max of %d\n", vxi, vyi, max_y);
                    break;
                }
                else {
                    x += vx;
                    y += vy;
                    if (y > max_y)
                         max_y = y;
                    vy--;
                    if (vx > 0) 
                        vx--;
                } 
            }
            total_hits += hits;
        }
    }    

    printf("Total possible trajectories: %d\n", total_hits);

    fclose(fd);

    return 0;
}
