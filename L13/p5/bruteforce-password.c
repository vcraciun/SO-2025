#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

#define MAX_LEN 6
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define BASE 26
#define PASSWORD "passwd"
#define N 14

int pipefd[2];
pid_t *children = NULL;
int target_found = 0;

void index_to_string(unsigned long long index, char *output) 
{
    int length = 1;
    unsigned long long total = 0;
    unsigned long long prev_total = 0;

    for (length = 1; length <= MAX_LEN; length++) 
    {
        unsigned long long add = 1;
        for (int i = 0; i < length; i++) add *= BASE;
        total += add;

        if (index < total) 
        {
            index -= prev_total;
            break;
        }

        prev_total = total;
    }

    for (int i = length - 1; i >= 0; i--) 
    {
        output[i] = ALPHABET[index % BASE];
        index /= BASE;
    }

    output[length] = '\0';
}

unsigned long long total_search_space() 
{
    unsigned long long total = 0;
    unsigned long long power = 1;
    for (int i = 1; i <= MAX_LEN; i++) 
    {
        power *= BASE;
        total += power;
    }
    return total;
}

void kill_all_children(int n) 
{
    for (int i = 0; i < n; i++) 
        kill(children[i], SIGTERM);
}

int main(int argc, char *argv[]) 
{
    if (pipe(pipefd) == -1) 
    {
        printf("pipe");
        return 1;
    }

    children = malloc(sizeof(pid_t) * N);
    if (!children) 
    {
        printf("malloc error");
        return 1;
    }

    unsigned long long total = total_search_space();

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    for (int i = 0; i < N; i++) 
    {
        unsigned long long start = total * i / N;
        unsigned long long end   = total * (i + 1) / N;

        pid_t pid = fork();
        if (pid == 0) 
        {
            close(pipefd[0]); 
            char str[MAX_LEN + 1];

            for (unsigned long long j = start; j < end; j++) 
            {
                index_to_string(j, str);
                if (strcmp(str, PASSWORD) == 0) 
                {
                    write(pipefd[1], str, strlen(str) + 1);
                    exit(0);
                }
            }

            exit(0);
        } 
        else 
            if (pid > 0) 
                children[i] = pid;
            else 
            {
                printf("fork error");
                free(children);
                return 1;
            }
    }    

    close(pipefd[1]);
    char buffer[MAX_LEN + 1];
    ssize_t n = read(pipefd[0], buffer, sizeof(buffer));

    if (n > 0) 
    {
        printf("Target found: %s\n", buffer);
        target_found = 1;
        kill_all_children(N);
    }

    for (int i = 0; i < N; i++) 
        waitpid(children[i], NULL, 0);

    free(children);
    close(pipefd[0]);

    if (!target_found) 
        printf("Target not found.\n");

    gettimeofday(&end_time, NULL);
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microseconds = end_time.tv_usec - start_time.tv_usec;
    if (microseconds < 0) 
    {
        seconds--;
        microseconds += 1000000;
    }
    printf("Elapsed time: %ld.%03ld seconds\n", seconds, microseconds);        

    return 0;
}