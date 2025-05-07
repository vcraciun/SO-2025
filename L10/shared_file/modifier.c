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
    int fd = open(FILE_PATH, O_RDWR);
    if (fd == -1)
        exit(EXIT_FAILURE);

    void *map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Modifier: \n%s\n", (char *)map);

    const char *new_message = "Modificat in SO2026!\n";
    memcpy(map, new_message, strlen(new_message));
    printf("Modifier: \n%s\n", new_message);

    munmap(map, FILE_SIZE);
    close(fd);

    return 0;
}