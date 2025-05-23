#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char buffer[20] = {0};
    if (argc == 2 && argv[1][0] != '-')
    {        
        int fib_count = atoi(argv[1]) - 2;
        sprintf(buffer, "%d", fib_count);
        setenv("n1", "0", 1);
        setenv("n2", "1", 1);
        setenv("n3", buffer, 1);
        setenv("n4", argv[1], 1);
        execl("/mnt/d/Facultate/_CURSURI_LABS/SO/2025/SO-2025/L12/ex41", "ex41", "-", NULL);
    }
    else
    {
        char* n3 = getenv("n3");
        int n33 = atoi(n3);
        if (n33 > 0) 
        {
            char* n1 = getenv("n1");
            char* n2 = getenv("n2");
            unsigned long long n11 = atol(n1);
            unsigned long long n22 = atol(n2);
            unsigned long long sum = n11 + n22;            
            sprintf(buffer, "%llu", sum);
            setenv("n1", n2, 1);
            setenv("n2", buffer, 1);
            sprintf(buffer, "%d", n33 - 1);
            setenv("n3", buffer, 1);
            execl("/mnt/d/Facultate/_CURSURI_LABS/SO/2025/SO-2025/L12/ex41", "ex41", "-", NULL);
        }
        else
        {
            char* n1 = getenv("n1");
            char* n2 = getenv("n2");
            char* count = getenv("n4");
            printf("FINISH\n");
            printf("FIB(%s)=%s\n", count, n2);
        }
    }
    return 0;
}
