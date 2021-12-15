
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define POLYMER_SIZE_MAX 100000 
#define RULES_MAX 256
#define SIM_STEPS 40 

struct Pair
{
    char a;
    char b;
};

int get_pair_index(struct Pair * pairs, int pairs_size, struct Pair target)
{
    for (int i = 0; i < pairs_size; i++)
        if (pairs[i].a == target.a && pairs[i].b == target.b)
            return i;
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

    char initial_state[POLYMER_SIZE_MAX];    
    int initial_size = 0;

    while (1)
    {
        char c = fgetc(fd);
        if (c == '\n') 
            break;
        else
            *(initial_state + initial_size++) = c;
    }   
    fgetc(fd);

    for (int i = 0; i < initial_size; i++)
        printf("%c", *(initial_state + i));
    printf("\n");
    

    struct Pair rules[RULES_MAX][3]; 
    int rules_size = 0;

    {
    char buff[3];
    while (!feof(fd))
    {
        if (fscanf(fd,"%c%c -> %c\n",buff + 0,      
                                     buff + 1,
                                     buff + 2) != 3)
            break;
        else {
            rules[rules_size][0] = (struct Pair){.a = buff[0], .b = buff[1]};
            rules[rules_size][1] = (struct Pair){.a = buff[0], .b = buff[2]};
            rules[rules_size][2] = (struct Pair){.a = buff[2], .b = buff[1]};
            rules_size++;
        }
    }
    }
    for (int i = 0; i < rules_size; i++)
        printf("Rule: %c%c -> %c%c,%c%c\n",
             rules[i][0].a,rules[i][0].b,
             rules[i][1].a,rules[i][1].b,
             rules[i][2].a,rules[i][2].b);    

    struct Pair pairs[RULES_MAX];
    size_t pair_tally_new[RULES_MAX];
    size_t pair_tally_old[RULES_MAX];
    int    pair_count = 0;
    
    for (int i = 0; i < initial_size-1; i++)
    {
        struct Pair current = (struct Pair){initial_state[i],initial_state[i+1]};
        if (get_pair_index(pairs, pair_count, current) == -1)
            pairs[pair_count++] = current;
        pair_tally_new[get_pair_index(pairs, pair_count, current)] += 1;
    }

    for (int i = 0; i < pair_count; i++)
        printf("%c%c - %ld\n", pairs[i].a, pairs[i].b, pair_tally_new[i]);

    size_t *cur = pair_tally_old;
    size_t *old = pair_tally_new;
    for (int step = 0; step < SIM_STEPS + 1; step++)
    {
        // printf("Step %d\n", step);
        // for (int i = 0; i < pair_count; i++)
        //     printf("%c%c - %ld\n", pairs[i].a, pairs[i].b, cur[i]);
        
        {size_t *temp = cur; cur = old; old = temp;}
        int pair_count_old = pair_count;
        for (int i = 0; i < pair_count_old; i++)
        {
            struct Pair *rule;
            for (int r = 0; r < rules_size; r++)
                if (rules[r][0].a == pairs[i].a && rules[r][0].b == pairs[i].b)
                    rule = rules[r];
            // printf("Rule: %c%c -> %c%c,%c%c\n", rule[0].a,rule[0].b,rule[1].a,rule[1].b, rule[2].a,rule[2].b);    

            if (get_pair_index(pairs, pair_count, rule[1]) == -1)
                pairs[pair_count++] = rule[1];
            cur[get_pair_index(pairs, pair_count, rule[1])] += old[get_pair_index(pairs, pair_count, rule[0])];
            
            if (get_pair_index(pairs, pair_count, rule[2]) == -1)
                pairs[pair_count++] = rule[2];
            cur[get_pair_index(pairs, pair_count, rule[2])] += old[get_pair_index(pairs, pair_count, rule[0])];
        }   
        for (int i = 0; i < pair_count_old; i++)         
            old[i] = 0;
    } 

    
    printf("Final Step\n");
    for (int i = 0; i < pair_count; i++)
        printf("%c%c - %ld\n", pairs[i].a, pairs[i].b, cur[i]);
        
    size_t element_tally[RULES_MAX];
    char elements[RULES_MAX];
    int element_cnt = 0;
    for (int i = 0; i < pair_count; i++)
    {
        int present = 0;
        for (int e = 0; e < element_cnt; e++)
            if (elements[e] == pairs[i].a) {
                element_tally[e] += cur[i];
                present = 1;
            }
        if (!present){
            elements[element_cnt] = pairs[i].a;
            element_tally[element_cnt] = cur[i];
            element_cnt++;
        }
    }            
    {
    int present = 0;
    for (int e = 0; e < element_cnt; e++)
        if (elements[e] == initial_state[initial_size-1]) {
            element_tally[e] += 1;
            present = 1;        
        }
    if (!present){
        elements[element_cnt] = initial_state[initial_size-1];
        element_tally[element_cnt] = 1;
        element_cnt++;
    }
    }
    for (int i = 0; i < element_cnt; i++)
        printf("Element |%c| is seen %ld times\n", elements[i], element_tally[i]);

    int max_index = 0;
    int min_index = 0;
    for (int e = 0; e < element_cnt; e++)
    {
        if (element_tally[e] > element_tally[max_index]) max_index = e;
        if (element_tally[e] < element_tally[min_index]) min_index = e;
    }
    printf("Min: %c, %ld\n", elements[min_index], element_tally[min_index]);
    printf("Max: %c, %ld\n", elements[max_index], element_tally[max_index]);
    printf("Diff: %ld\n", element_tally[max_index] - element_tally[min_index]);

    // printf("Max: %d, Min: %d, Diff: %d\n", max_count, min_count, max_count-min_count);

    // for (int i = 0; i < unique_chars_count; i++)
    //     printf("Element %c occurs %d times.\n", unique_chars[i], unique_chars_tally[i]);

    fclose(fd);
 
    return 0;
}
