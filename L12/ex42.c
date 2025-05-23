#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    pid_t pid = fork();

    if (pid < 0) 
    {
        printf("Fork failed!\n");
        return 1;
    }

    if (pid == 0) 
    {
        printf("Executing 'ls -l'\n");
        execl("/bin/ls", "ls", NULL);
        printf("execl failed");
        exit(1);
    } 
    else 
    {
        printf("Wait for child...\n");
        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    }

    return 0;
}