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

    const char *message = "Hello SO 2025!\n";
    memcpy(map, message, strlen(message));

    printf("Starter: \n%s\n", message);
    printf("Starter: .........\n");
    getchar();

    munmap(map, FILE_SIZE);
    close(fd);

    return 0;
}