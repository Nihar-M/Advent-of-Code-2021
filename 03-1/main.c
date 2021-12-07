#include <stdio.h>

int main()
{
    char file[] = "input.txt";

    FILE* fd = fopen(file,"r");
    if (fd == NULL) {
        printf("Error occured opening file %s.\n",file );
        return 1;
    }

    int counter[12] = {0};
    char current[13];
    (void) counter;    
    
    while (!feof(fd))
    {
        fscanf(fd,"%s12",current);
	for (int i = 0; i < 12; i++)
	{
	    counter[i] += (current[i] == '1') ? 1 : -1;
	}
    }

    int gamma_rate = 0;   

    for (int i = 0; i < 12; i++)
    {
 	printf("%c",(counter[i]<0) ? '0' : '1');
	gamma_rate <<= 1;
	gamma_rate += (counter[i] > 0);
    }

    int epsilon_rate = (1 << 12)-1  - gamma_rate; 

    printf("Gamma Rate: %d\n", gamma_rate);
    printf("Epsilon Rate: %d\n", epsilon_rate);
    printf("Power Usage: %d\n", gamma_rate * epsilon_rate);
    fclose(fd);

    // printf("Sum = %d", sum);
    return 0;
}
