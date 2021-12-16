
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BIT_MAX 8192
#define SUBPACK_MAX 16

FILE *fd;

int bits_read = 0;

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

long read_bits(int num)
{
    long res = 0;
    for (int i = num-1; i >= 0; i--)
        res |= (get_bit() << i);
    return res;
}

long read_literal();
long read_operator();
long read_package();

long read_literal()
{
    long res = 0;  
    while (get_bit()) {
        res += read_bits(4);
        res = res << 4;
    }
    res += read_bits(4);
    // printf("Read Literal: %ld\n", res);
    return res;
}

long read_operator(int type)
{
    long res;
    switch (type)
    {
        case 7:
            read_bits(get_bit() ? 11:15);
            return (read_package() == read_package());
        case 6:
            read_bits(get_bit() ? 11:15);
            return (read_package() < read_package());
        case 5:
            read_bits(get_bit() ? 11:15);
            return (read_package() > read_package());
        case 3:
            if (get_bit())
            {
                int number_of_subpackets = read_bits(11);
                res = read_package();
                for (int i = 1; i < number_of_subpackets; i++)
                {
                    long val = read_package();
                    if (val > res) res = val;
                }
            }
            else
            {
                int subpackets_continue_till = bits_read + read_bits(15);
                res = read_package();
                while (bits_read < subpackets_continue_till)
                {
                    long val = read_package(); 
                    if (val > res) res = val;
                }
            }
            break;
        case 2:
            if (get_bit())
            {
                int number_of_subpackets = read_bits(11);
                res = read_package();
                for (int i = 1; i < number_of_subpackets; i++)
                {
                    long val = read_package();
                    if (val < res) res = val;
                }
            }
            else
            {
                int subpackets_continue_till = bits_read + read_bits(15);
                res = read_package();
                while (bits_read < subpackets_continue_till)
                {
                    long val = read_package(); 
                    if (val < res) res = val;
                }
            }
            break;
        case 1:
            if (get_bit())
            {
                int number_of_subpackets = read_bits(11);
                res = read_package();
                for (int i = 1; i < number_of_subpackets; i++)
                {
                    long val = read_package();
                    res *= val;
                }
            }
            else
            {
                int subpackets_continue_till = bits_read + read_bits(15);
                res = read_package();
                while (bits_read < subpackets_continue_till)
                {
                    long val = read_package(); 
                    res *= val;
                }
            }
            break;
        case 0:
            if (get_bit())
            {
                int number_of_subpackets = read_bits(11);
                res = read_package();
                for (int i = 1; i < number_of_subpackets; i++)
                {
                    long val = read_package();
                    res += val;
                }
            }
            else
            {
                int subpackets_continue_till = bits_read + read_bits(15);
                res = read_package();
                while (bits_read < subpackets_continue_till)
                {
                    long val = read_package(); 
                    res += val;
                }
            }
            break;
    }
    return res;
}




long read_package()
{
    // printf("Version: %ld\n", read_bits(3));
    read_bits(3);

    int type = read_bits(3);
    if (type == 4) {
        // printf("Reading Literal\n");
        return read_literal();
    }
    else {
        // printf("Reading Operator: %d\n", type);
        return read_operator(type);
    }
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

    printf("Evaluation: %ld\n", read_package());
    

    fclose(fd);
 
    return 0;
}
