
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

struct Pair
{
    bool l_filled;
    bool l_isNum;
    int l_value;
    struct Pair *l_pair;

    bool r_filled;
    bool r_isNum;
    int r_value;
    struct Pair *r_pair;
};



bool add_pair(struct Pair* base)
{
    // left is empty
    if (!base->l_filled) {
        base->l_filled = true;
        base->l_isNum = false;
        base->l_pair = calloc(1, sizeof(struct Pair));
        return true;
    }
    // left has a pair in it
    if (!base->l_isNum) {
        // suceeded add_pair in left pair/branch
        if (add_pair(base->l_pair))
            return true;
    }
    // right is empty
    if (!base->r_filled) {
        base->r_filled = true;
        base->r_isNum = false;
        base->r_pair = calloc(1, sizeof(struct Pair));
        return true;
    }
    // right has a pair in it
    if (!base->r_isNum) {
        // suceeded add_pair in left pair/branch
        if (add_pair(base->r_pair))
            return true;
    }
    // printf("Could not insert pair\n");
    return false;
}

bool add_num(struct Pair* base, int value)
{
    // left is empty
    if (!base->l_filled) {
        base->l_filled = true;
        base->l_isNum = true;
        base->l_value = value; 
        return true;
    }
    // left has a pair in it
    if (!base->l_isNum) {
        // suceeded add_num in left pair/branch
        if (add_num(base->l_pair,value))
            return true;
    }
    // right is empty
    if (!base->r_filled) {
        base->r_filled = true;
        base->r_isNum = true;
        base->r_value = value;
        return true;
    }
    // right has a pair in it
    if (!base->r_isNum) {
        // suceeded add_num in left pair/branch
        if (add_num(base->r_pair,value))
            return true;
    }
    // printf("Could not insert number\n");
    return false;
}

void indent(int times)
{
    for (int i = 0; i < times; i++) printf("  ");
}

void print_flat(struct Pair *base, int depth)
{
    printf("[");
    if (base->l_filled) {
        if (base->l_isNum) {
            printf("%d",base->l_value);
        }
        else
            print_flat(base->l_pair, depth+1);
    }
    printf(",");
    if (base->r_filled){
        if (base->r_isNum) {
            printf("%d",base->r_value);
        }
        else
            print_flat(base->r_pair, depth+1);
    }
    printf("]");
}


void print_pair(struct Pair *base, int depth)
{
    indent(depth);
    printf("[\n");
    if (base->l_filled) {
        if (base->l_isNum) {
            indent(depth + 1);
            printf("%d\n",base->l_value);
        }
        else
            print_pair(base->l_pair, depth+1);
    }
    indent(depth);
    printf(",\n");
    if (base->r_filled){
        if (base->r_isNum) {
            indent(depth + 1);
            printf("%d\n",base->r_value);
        }
        else
            print_pair(base->r_pair, depth+1);
    }
    indent(depth);
    printf("]\n");
}


bool split_num(struct Pair *base)
{
    if (base->l_isNum) {
        if (base->l_value > 9) {
            base->l_isNum = false;
            struct Pair *next = calloc(1, sizeof(struct Pair));
            base->l_pair = next;
            
            next->l_filled = true;
            next->l_isNum = true;
            next->l_value = base->l_value/2;
                    
            next->r_filled = true;
            next->r_isNum = true;
            next->r_value = base->l_value - next->l_value; 
            
            base->l_value = 0;
            return true;
        }
    }
    else
        if (split_num(base->l_pair))
            return true;
    if (base->r_isNum) {
        if (base->r_value > 9) {
            base->r_isNum = false;
            struct Pair *next = calloc(1, sizeof(struct Pair));
            base->r_pair = next;
            
            next->l_filled = true;
            next->l_isNum = true;
            next->l_value = base->r_value/2;
                    
            next->r_filled = true;
            next->r_isNum = true;
            next->r_value = base->r_value - next->l_value; 
            
            base->r_value = 0;
            return true;
        }
    }
    else
        if (split_num(base->r_pair))
            return true;
    return false;
}

void inc_num(struct Pair *base, int* index, int* value)
{
    // left side
    if (base->l_isNum) {
        if (*index == 0)
            base->l_value += *value;
        (*index)--;
    }
    else
        inc_num(base->l_pair, index, value);
    if (base->r_isNum) {
        if (*index == 0)
            base->r_value += *value;
        (*index)--;
    }
    else
        inc_num(base->r_pair, index, value);

}

void add_to_index(struct Pair *base, int index, int value)
{
    inc_num(base, &index, &value);
}

void explode_num(struct Pair *base, int* index, int depth, bool *exploded, int *left, int *right)
{
    if (!(*exploded))
    {
        if (base->l_isNum)
            (*index)++;
        else 
        {
            if (depth == 3) {
                // explode it
                // printf("Exploding\n");
                // print_pair(base->l_pair, 0);
                assert(base->l_pair->l_isNum && base->l_pair->r_isNum);
                *left = base->l_pair->l_value; 
                *right = base->l_pair->r_value; 
                base->l_isNum = true;
                base->l_value = 0;
                free(base->l_pair);
                *exploded = true;
            }
            else
                explode_num(base->l_pair, index, depth + 1, exploded, left, right);
        }
    }
    if (!(*exploded))
    {
        if (base->r_isNum)
            (*index)++;
        else
        {
            if (depth == 3) {
                // printf("Exploding\n");
                // print_pair(base->r_pair, 0);
                assert(base->r_pair->l_isNum && base->r_pair->r_isNum);
                *left = base->r_pair->l_value; 
                *right = base->r_pair->r_value; 
                base->r_isNum = true;
                base->r_value = 0;
                free(base->r_pair);
                *exploded = true;
            }
            else
                explode_num(base->r_pair, index, depth + 1, exploded, left, right);
        }
    }
}     
  

bool explode(struct Pair *base)
{
    int index = 0;
    bool exploded = false;
    int left;
    int right;
    explode_num(base, &index, 0, &exploded, &left, &right);    
    // printf("Left %d, Right %d, Index: %d\n", left, right, index);
    // printf("Exploded %d\n", exploded);
    if (exploded) {
        add_to_index(base, index-1, left);
        add_to_index(base, index+1, right);
    }
    return exploded;
}

struct Pair* read_line(FILE* fd)
{
    if (feof(fd)) return NULL;
    struct Pair *base = calloc(1, sizeof(struct Pair));
    while (1)
    {
        char c = fgetc(fd);
        if (c == '\n')
        {
            return base;
        }
        if (feof(fd)) return NULL; 
        switch (c)
        {
            case '[':
                // printf("starting pair\n");
                add_pair(base);
                break;
            case ']':
                // printf("ending pair\n");
                break;
            case ',':
                // printf("next item in pair\n");
                break;
            default:
                if ('0' <= c && c <= '9') {
                    // printf("number: %d\n", c-'0');
                    add_num(base, c - '0');
                }
                else {}
                    // printf("error\n");
        }
    }
}

void reduce(struct Pair* base)
{
    bool changed = true; 
    while (changed){

        if (explode(base->l_pair))
            changed = true;
        else
            if (split_num(base->l_pair))
                changed = true;
            else
                changed = false;
        // print_flat(base->l_pair, 0); printf("\n");
    }
}

int magnitude(struct Pair* base)
{
    int left;
    if (base->l_isNum)
        left = base->l_value;
    else
        left = magnitude(base->l_pair);
    int right;
    if (base->r_isNum)
        right = base->r_value;
    else
        right = magnitude(base->r_pair);
    return 3*left + 2*right;
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

    struct Pair *base = read_line(fd);
    reduce(base);   

    while (1)
    {
        struct Pair *new_base = read_line(fd);
        if (new_base == NULL) break;
        struct Pair *old_base = base;
        base = calloc(1, sizeof(struct Pair));
        base->l_pair = old_base;
        old_base->r_pair = new_base->l_pair;      
        old_base->l_filled = true;
        old_base->l_isNum = false;
        old_base->r_filled = true;
        old_base->r_isNum = false; 
        // print_flat(base->l_pair, 0); printf("\n"); 
        reduce(base);   
        // print_flat(base->l_pair, 0); printf("\n"); 
    }
    
    printf("Magnitude: %d\n", magnitude(base->l_pair));
     
    /*
    print_flat(base->l_pair, 0); printf("\n");
    explode(base->l_pair);
    print_flat(base->l_pair, 0); printf("\n");
    explode(base->l_pair);
    print_flat(base->l_pair, 0); printf("\n");
    */
    fclose(fd);
 
    return 0;
}
