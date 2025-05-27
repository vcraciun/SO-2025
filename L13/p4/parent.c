#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define STRING_LEN 8
#define NUM_STRINGS 10
#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"

void generate_random_string(char *str, size_t length) 
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length; ++i)
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    str[length] = '\0';
}

int main() 
{
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    int write_fd = open(FIFO1, O_WRONLY);
    int read_fd = open(FIFO2, O_RDONLY);

    if (write_fd < 0 || read_fd < 0) 
    {
        printf("open");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (int i = 0; i < NUM_STRINGS; ++i) 
    {
        int sum = 0;
        char str[STRING_LEN + 1];

        generate_random_string(str, STRING_LEN);
        write(write_fd, str, STRING_LEN + 1);
        read(read_fd, &sum, sizeof(sum));
        printf("Parent sent: %s, received sum: %d\n", str, sum);
    }

    close(write_fd);
    close(read_fd);

    return 0;
}