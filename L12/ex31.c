#include <stdio.h>
#include <unistd.h>

int main() 
{
    printf("main()\n");
    execl("./ex32", "ex32", "30", "70", NULL);
    perror("Something went wrong!\n");
    return 1;
}
