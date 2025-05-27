#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define STRING_LEN 8
#define NUM_STRINGS 10
#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"

int main() {
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    int read_fd = open(FIFO1, O_RDONLY);
    int write_fd = open(FIFO2, O_WRONLY);

    if (read_fd < 0 || write_fd < 0) 
    {
        printf("open");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_STRINGS; ++i) 
    {
        char str[STRING_LEN + 1];
        int sum = 0;

        read(read_fd, str, STRING_LEN + 1);
        for (int j = 0; j < STRING_LEN; ++j) 
            sum += str[j];
        //sleep(1);
        write(write_fd, &sum, sizeof(sum));
    }

    close(read_fd);
    close(write_fd);

    return 0;
}