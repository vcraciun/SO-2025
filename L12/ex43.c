#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    char buffer[20] = {0};
    if (argc == 2 && argv[1][0] != '-')
    {        
        int fd = open("share.txt", O_RDWR | O_CREAT, 0666);
        unsigned long long* shared = mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
        int fib_count = atoi(argv[1]) - 2;
        memset(shared, 0, 0x1000);
        shared[0] = 0;
        shared[1] = 1;
        shared[2] = fib_count;
        execl("/mnt/d/Facultate/_CURSURI_LABS/SO/2025/SO-2025/L12/ex41", "ex41", "-", NULL);
    }
    else
    {
        int fd = open("share.txt", O_RDWR, 0666);
        unsigned long long* shared = mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
        if (shared[2] > 0) 
        {
            unsigned long long n1 = shared[0];
            unsigned long long n2 = shared[1];
            unsigned long long sum = n1 + n2;            
            shared[2]--;
            execl("/mnt/d/Facultate/_CURSURI_LABS/SO/2025/SO-2025/L12/ex41", "ex41", "-", NULL);
        }
        else
        {
            printf("FINISH\n");
            printf("FIB(%llu)=%llu\n", shared[2], shared[1]);
        }
    }
    return 0;
}