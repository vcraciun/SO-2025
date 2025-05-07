#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_PATH "shared_file.dat"
#define FILE_SIZE 1024

int main() {
    int fd = open(FILE_PATH, O_RDWR | O_CREAT, 0666);
    if (fd == -1)
        exit(EXIT_FAILURE);

    if (ftruncate(fd, FILE_SIZE) == -1)
        exit(EXIT_FAILURE);

    void *map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    memset(map, 0, FILE_SIZE);
    if (map == MAP_FAILED) {
        close(fd);
        exit(EXIT_FAILURE);
    }

    int old_size = 0;
    printf("Monitor: Astept comenzi...\n");
    printf("Monitor: Pentru iesire, introdu 'exit'!\n");

    while (1) {
        int new_size = strlen((char*)map);
        if (new_size != old_size && new_size > 1) {
            old_size = new_size;
            printf("Am primit: [%s]\n", (char*)map);
            if (!strcmp(map, "exit")) {
                printf("Exiting...\n");
                break;
            }
            memset(map, 0, FILE_SIZE);
        }
    }

    munmap(map, FILE_SIZE);
    close(fd);

    return 0;
}