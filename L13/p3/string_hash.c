#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define STRING_LEN 8
#define NUM_STRINGS 10

void generate_random_string(char *str, size_t length) 
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length; ++i) 
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    str[length] = '\0';
}

int main() 
{
    int parent_child[2]; 
    int child_parent[2]; 

    if (pipe(parent_child) == -1 || pipe(child_parent) == -1) 
    {
        printf("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) 
    {
        printf("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) 
    {
        close(parent_child[0]); //parintele nu citeste din proprioul pipe
        close(child_parent[1]); //parintele nu scrie in pipe-ul destinat child-ului

        srand(time(NULL));

        for (int i = 0; i < NUM_STRINGS; ++i) 
        {
            char str[STRING_LEN + 1] = {0};
            int sum;
            generate_random_string(str, STRING_LEN);
            write(parent_child[1], str, STRING_LEN + 1);        
            read(child_parent[0], &sum, sizeof(sum));
            printf("Parent sent: %s, received sum: %d\n", str, sum);
        }

        close(parent_child[1]);
        close(child_parent[0]);

        wait(NULL);
    } 
    else 
    {
        close(child_parent[0]); //child nu citeste din propriul pipe
        close(parent_child[1]); //child nu scrie in pipe-ul parintelui

        for (int i = 0; i < NUM_STRINGS; ++i) 
        {
            char str[STRING_LEN + 1];
            read(parent_child[0], str, STRING_LEN + 1);

            int sum = 0;
            for (int j = 0; j < STRING_LEN; ++j) 
                sum += str[j];

            //sleep(1);
            write(child_parent[1], &sum, sizeof(sum));
        }

        close(parent_child[0]);
        close(child_parent[1]);
        exit(0);
    }

    return 0;
}
