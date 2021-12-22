
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define MAX_RANGES 500

#define MIN_COORD -50
#define MAX_COORD -MIN_COORD + 1

struct Range
{
    bool on;
    int sx, ex, sy, ey, sz, ez;
};

bool in_range(struct Range r, int x, int y, int z)
{
    return (r.sx <= x && x <= r.ex) && (r.sy <= y && y <= r.ey) && (r.sz <= z && z <= r.ez);
}

void p_range(struct Range r)
{
    printf("(%4s )x=%d..%d,y=%d..%d,z=%d..%d\n", r.on ? "on":"off" , r.sx, r.ex, r.sy, r.ey, r.sz, r.ez);
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

    struct Range ranges[MAX_RANGES];
    (void)ranges;
    int ranges_sz = 0;

    for (; ranges_sz < MAX_RANGES && !feof(fd); ranges_sz++)
    {
        char on_off[4];

        fscanf(fd, "%3s x=%d..%d,y=%d..%d,z=%d..%d\n", on_off, &ranges[ranges_sz].sx, &ranges[ranges_sz].ex, &ranges[ranges_sz].sy, &ranges[ranges_sz].ey, &ranges[ranges_sz].sz, &ranges[ranges_sz].ez);
        if (!strncmp("off",on_off,3))    ranges[ranges_sz].on = false;
        else                            ranges[ranges_sz].on = true;
    }

    // printf("in range %d\n", in_range(ranges[0], 12, 12, 12));

    printf("Found %d ranges\n", ranges_sz);

    for (int r = 0; r < ranges_sz; r++)
    {
        p_range(ranges[r]);
    }

    int are_on = 0;
    for (int x = MIN_COORD; x < MAX_COORD; x++)
    {
        for (int y = MIN_COORD; y < MAX_COORD; y++)
        {
            for (int z = MIN_COORD; z < MAX_COORD; z++)
            {
                bool is_on = false;
                for (int r = 0; r < ranges_sz; r++)
                {
                    if (is_on) {
                        if (ranges[r].on) {continue;}
                        if (in_range(ranges[r], x, y, z)) {is_on = false;}
                    }
                    else { // !is_on
                        if (!ranges[r].on) {continue;}
                        if (in_range(ranges[r], x, y, z)) {is_on = true;}
                    }
                }

                // printf("(%d,%d,%d) is (%4s )\n", x,y,z, is_on?"on":"off");
                are_on += is_on;
                // return 0;
            }
            // printf("\tfinished y=%d\n", y);
        }
        // printf("finished x=%d\n", x);
    }

    printf("%d cells are on\n", are_on);


    fclose(fd);
 
    return 0;
}