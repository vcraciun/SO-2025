#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        printf("Invalid args!\n");
        return 1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int result = a * b;

    printf("product: %d * %d = %d\n", a, b, result);

    char res_str[16], exp_str[16];
    snprintf(res_str, sizeof(res_str), "%d", result);
    snprintf(exp_str, sizeof(exp_str), "%d", 2);

    execl("./ex34", "ex34", res_str, exp_str, NULL);
    perror("Something went wrong!\n");
    return 1;
}