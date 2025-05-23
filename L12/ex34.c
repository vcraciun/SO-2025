#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        printf("Invalid args!\n");
        return 1;
    }

    int base = atoi(argv[1]);
    int exp = atoi(argv[2]);
    int result = (int)pow(base, exp);

    printf("power: %d ^ %d = %d\n", base, exp, result);
    printf("FINISH\n");
    return 0;
}