#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LEN 10
#define BASE 26
const char *ALPHABET = "abcdefghijklmnopqrstuvwxyz";

void index_to_string(unsigned long long index, char *output) 
{
    unsigned long long total = 0, prev_total = 0;
    int length;

    for (length = 1; length <= MAX_LEN; length++) 
    {
        unsigned long long add = 1;
        for (int i = 0; i < length; i++) 
            add *= BASE;
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

int main(int argc, char *argv[]) 
{
    if (argc != 5) 
    {
        fprintf(stderr, "Usage: %s <target> <start> <end> <fifo_path>\n", argv[0]);
        return 1;
    }

    char *target = argv[1];
    unsigned long long start = strtoull(argv[2], NULL, 10);
    unsigned long long end = strtoull(argv[3], NULL, 10);
    char *fifo_path = argv[4];

    char candidate[MAX_LEN + 1];
    for (unsigned long long i = start; i < end; i++) 
    {
        index_to_string(i, candidate);
        if (strcmp(candidate, target) == 0) 
        {
            int fd = open(fifo_path, O_WRONLY);
            write(fd, candidate, strlen(candidate) + 1);
            close(fd);
            return 0;
        }
    }

    return 0;
}