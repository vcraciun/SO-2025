// parent.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>

#define MAX_CHILDREN 100
#define MAX_LEN 6
#define BASE 26
#define FIFO_PATH "/tmp/brute_fifo"

const char *CHILD_EXEC = "./child"; 

unsigned long long total_search_space() 
{
    unsigned long long total = 0, power = 1;
    for (int i = 1; i <= MAX_LEN; i++) 
    {
        power *= BASE;
        total += power;
    }
    return total;
}

void kill_all(pid_t *pids, int n) 
{
    for (int i = 0; i < n; ++i)
        kill(pids[i], SIGTERM);
}

int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        printf("Usage: %s <target> <num_processes>\n", argv[0]);
        return 1;
    }

    char *target = argv[1];
    int num_procs = atoi(argv[2]);
    if (num_procs < 1 || num_procs > MAX_CHILDREN) 
    {
        printf("Invalid number of processes (1-%d)\n", MAX_CHILDREN);
        return 1;
    }

    unlink(FIFO_PATH);
    if (mkfifo(FIFO_PATH, 0666) == -1) 
    {
        printf("mkfifo");
        return 1;
    }

    unsigned long long total = total_search_space();

    pid_t children[MAX_CHILDREN];
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < num_procs; ++i) 
    {
        unsigned long long start_idx = total * i / num_procs;
        unsigned long long end_idx = total * (i + 1) / num_procs;

        pid_t pid = fork();
        if (pid == 0) 
        {
            char start_str[32], end_str[32];
            snprintf(start_str, sizeof(start_str), "%llu", start_idx);
            snprintf(end_str, sizeof(end_str), "%llu", end_idx);
            execl(CHILD_EXEC, CHILD_EXEC, target, start_str, end_str, FIFO_PATH, NULL);
            perror("execl");
            exit(1);
        } 
        else 
            if (pid > 0)
                children[i] = pid;
            else 
            {
                printf("fork");
                return 1;
            }
    }

    int fifo_fd = open(FIFO_PATH, O_RDONLY);
    char buffer[MAX_LEN + 1];
    if (read(fifo_fd, buffer, sizeof(buffer)) > 0) 
    {
        gettimeofday(&end, NULL);
        long seconds = end.tv_sec - start.tv_sec;
        long micros = end.tv_usec - start.tv_usec;
        if (micros < 0) 
        {
            seconds--;
            micros += 1000000;
        }

        printf("Target found: %s\n", buffer);
        printf("Elapsed time: %ld.%06ld seconds\n", seconds, micros);

        kill_all(children, num_procs);
    }

    close(fifo_fd);
    unlink(FIFO_PATH);

    for (int i = 0; i < num_procs; ++i) 
        waitpid(children[i], NULL, 0);

    return 0;
}