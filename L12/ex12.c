#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    setenv("VAR_FROM_PARENT", "SO-2025", 1);
    execl("./ex11", "ex11", NULL);
    printf("Something went wrong!\n");
    return 1;
}
