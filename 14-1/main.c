
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define POLYMER_SIZE_MAX 100000 
#define RULES_MAX 256
#define SIM_STEPS 10

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }

    // the strands
    char strand_a[POLYMER_SIZE_MAX];
    char strand_b[POLYMER_SIZE_MAX];

    char* current = strand_a;
    int current_size = 0;
    (void)current;
    (void)current_size;
    char* next = strand_b;
    int next_size = 0;
    
    while (1)
    {
        char c = fgetc(fd);
        if (c == '\n') 
            break;
        else {
            *(next + next_size) = c;
            next_size++;
        } 
    }   
    fgetc(fd);

    {char* temp = current; current = next; next = temp;}
    {current_size = next_size; next_size = 0;}
 
    for (int i = 0; i < current_size; i++)
        printf("%c", *(current + i));
    printf("\n");
    

    char rules[RULES_MAX][3]; 
    int rules_size = 0;

    while (!feof(fd))
    {
        if (fscanf(fd,"%c%c -> %c\n",rules[rules_size] + 0,      
                                     rules[rules_size] + 1,
                                     rules[rules_size] + 2) != 3)
            break;
        else
            rules_size++;
    }
    
    // for (int i = 0; i < rules_size; i++)
    //     printf("Rule: %c%c -> %c\n", rules[i][0],rules[i][1],rules[i][2]);    


    for (int step = 0; step < SIM_STEPS; step++)
    {
        assert(next_size == 0);
        
        for (int i = 0; i < current_size -1; i++)      
        {
            char *chosen_rule = NULL;
            for (int r = 0; r < rules_size; r++)
            {
                if (rules[r][0] == current[i] && rules[r][1] == current[i+1])
                    chosen_rule = (char*)(rules + r);
            }
            assert(chosen_rule != NULL);
            next[next_size++] = chosen_rule[0];
            next[next_size++] = chosen_rule[2];
            // next[next_size++] = chosen_rule[1];
        }
        next[next_size++] = current[current_size-1];
        {char* temp = current; current = next; next = temp;}
        {current_size = next_size; next_size = 0;}
        printf("After step %d there are %d atoms\n", step + 1, current_size); 
        // for (int i = 0; i < current_size; i++)
        //     printf("%c", *(current + i));
        printf("\n");
    } 

    char unique_chars[RULES_MAX];
    int unique_chars_tally[RULES_MAX];
    int unique_chars_count = 0;    

    for (int i = 0; i < current_size; i++)
    {
        int present = 0;
        for (int j = 0; j < unique_chars_count; j++)
            if (current[i] == unique_chars[j]) { 
                present = 1;
                unique_chars_tally[j]++;
            }
        if (!present) {
            unique_chars[unique_chars_count] = current[i];
            unique_chars_tally[unique_chars_count] = 1;
            unique_chars_count++;
        }
    }
    
    int max_count = 1;
    int min_count = 1000000;
    for (int i = 0; i < unique_chars_count; i++)
    {
        if (unique_chars_tally[i] > max_count)
            max_count = unique_chars_tally[i];
        if (unique_chars_tally[i] < min_count)
            min_count = unique_chars_tally[i];
    }
    
    printf("Max: %d, Min: %d, Diff: %d\n", max_count, min_count, max_count-min_count);

    // for (int i = 0; i < unique_chars_count; i++)
    //     printf("Element %c occurs %d times.\n", unique_chars[i], unique_chars_tally[i]);

    fclose(fd);
 
    return 0;
}
