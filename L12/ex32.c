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
    int result = a + b;

    printf("sum: %d + %d = %d\n", a, b, result);

    char res_str[16], b_str[16];
    snprintf(res_str, sizeof(res_str), "%d", result);
    snprintf(b_str, sizeof(b_str), "%d", 4);

    execl("./ex33", "ex33", res_str, b_str, NULL);
    perror("Something went wrong!\n");
    return 1;
}