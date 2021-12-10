
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LINE_CAP 1024

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
    
    int error_score = 0;

    int error_counts[4];
    for (int i = 0; i < 4; i++) error_counts[i] = 0;

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
   

        // printf("Fail character: |%c|\n", fail);
        switch (fail)
        {
            case ')': error_score += 3; error_counts[0]++; break;
            case ']': error_score += 57; error_counts[1]++; break;
            case '}': error_score += 1197; error_counts[2]++; break;
            case '>': error_score += 25137; error_counts[3]++; break;
        }

        // printf("Error Counts: %d,%d,%d,%d\n", error_counts[0], error_counts[1], error_counts[2], error_counts[3]);
    }
    
    printf("Total Error score: %d\n", error_score);

    fclose(fd);
 
    return 0;
}
