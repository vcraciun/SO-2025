#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    pid = fork();
    
    if (pid < 0) {
        perror("Error");
        return 1;
    } 
    else 
        if (pid == 0)
            printf("EU(child) = %d, parent = %d\n", getpid(), getppid());
        else
            printf("EU(parent) = %d, child = %d\n", getpid(), pid);

    return 0;
}
