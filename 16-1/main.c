
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BIT_MAX 8192

FILE *fd;

int bits_read = 0;
int version_sum = 0;

int next_bit(int clear)
{
    static char hex = 0;
    static char bit_num = 0;
    bit_num = (bit_num)%4;
    if (clear){
        bits_read += (4-bit_num)%4;
        bit_num = 0;
        return -1;
    } 
    bits_read++;
    switch(bit_num)
    {
        case 0:
            hex = fgetc(fd);
            // printf("\n|%c|", hex);
            if ('0' <= hex && hex <= '9')
                hex -= '0';
            else if ('A' <= hex && hex <= 'F')
                hex -= 'A' - 10;
            bit_num++;
            return (hex >> 3) & 0x1;
        case 1:
            bit_num++;
            return (hex >> 2) & 0x1;
        case 2:
            bit_num++;
            return (hex >> 1) & 0x1;
        case 3:
            bit_num++;
            return (hex >> 0) & 0x1;
        default:
            printf("fatal error in reading bits\n"); 
            return -1;
    }   
}

#define get_bit() next_bit(0)
#define clear_padding() next_bit(1)

int read_bits(int num)
{
    int res = 0;
    for (int i = num-1; i >= 0; i--)
        res |= (get_bit() << i);
    return res;
}

int read_literal();
int read_operator();
int read_package();

int read_literal()
{
    int res = 0;  
    while (get_bit()) {
        res += read_bits(4);
        res = res << 4;
    }
    res += read_bits(4);
    return res;
}

int read_operator()
{
    if (get_bit())
    {
        int number_of_subpackets = read_bits(11);
        // printf("type 1: %d\n", number_of_subpackets);
        for (int i = 0; i < number_of_subpackets; i++)
            read_package();
    }
    else
    {   
        int subpackets_continue_till = bits_read + read_bits(15);
        // printf("type 0: %d\n", subpackets_continue_till);
        while (bits_read < subpackets_continue_till)
            read_package();
    }
    return 0;
}

int read_package()
{
    version_sum += read_bits(3);
    if (read_bits(3) == 4) {
        // printf("Reading Literal\n");
        read_literal();
    }
    else {
        // printf("Reading Operator\n");
        read_operator();
    }
    return 0;
}


int main()
{
    char file[] = "input.txt";

    fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("Error occured opening file %s.\n", file);
        return 1;
    }

    read_package();
    
    printf("Sum of versions: %d\n", version_sum);

    fclose(fd);
 
    return 0;
}
