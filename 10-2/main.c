
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LINE_CAP 1024
#define SCORE_CAP 128

int main()
{
    char file[] = "input.txt";

    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }

    char linebuffer[LINE_CAP];    
    
    char stack[LINE_CAP];
    int stack_size;
    
    size_t scores[SCORE_CAP];
    int scores_size = 0; 

    while (!feof(fd))
    {
        fgets(linebuffer, LINE_CAP, fd);
        // printf("Line: %s\n", linebuffer);
        stack_size = 0;
        int string_len = strlen(linebuffer) - 1;
        

        char fail = 0;
        for (int i = 0; i < string_len; i++)
        {
            char c = linebuffer[i];
            if (stack_size >= LINE_CAP -2) assert( 0 && "stack capacity exceeded");
            if (c == '[' || c == '{' || c == '<' || c == '(')
                stack[stack_size++] = c;
            else
            {
                if (c == ']')
                    if (stack[stack_size-1] == '['){stack_size--;}
                    else {fail = ']'; break;}
                else if (c == '}')
                    if (stack[stack_size-1] == '{'){stack_size--;}
                    else {fail = '}'; break;}
                else if (c == '>')
                    if (stack[stack_size-1] == '<'){stack_size--;}
                    else {fail = '>'; break;}
                else if (c == ')')
                    if (stack[stack_size-1] == '('){stack_size--;}
                    else {fail = ')'; break;}
                else printf("Unreachable\n");
            }
        }
            
        if (!fail)
        {   
            size_t completion_score = 0;
            while(stack_size > 0)
            {
                completion_score *= 5;
                switch (stack[--stack_size])
                {
                    case '(': completion_score += 1; break;
                    case '[': completion_score += 2; break;
                    case '{': completion_score += 3; break;
                    case '<': completion_score += 4; break;
                }
            }
            if (completion_score != 0)
                scores[scores_size++] = completion_score;
        }
        
        // printf("Fail character: |%c|\n", fail);
        // printf("Error Counts: %d,%d,%d,%d\n", error_counts[0], error_counts[1], error_counts[2], error_counts[3]);
    }
    
    for (int i = 0; i < scores_size; i++) 
    {
        for (int j = 0; j < scores_size; j++)
        {
            if (scores[i] < scores[j])
            {
                size_t temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }    
    }
    
    // for (int i = 0; i < scores_size; i++)
    // {
    //     printf("Score: %ld\n", scores[i]);
    // }
    
    printf("Middle Score: %ld\n", scores[scores_size/2]);

    fclose(fd);
 
    return 0;
}
