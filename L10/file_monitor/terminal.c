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

    printf("Terminal: Pentru a iesi, introduceti exit!\n");
    while (1) {
        char command[100];
        printf("CMD> ");
        fgets(command, sizeof(command), stdin);        
        command[strcspn(command, "\n")] = 0;
        memcpy(map, command, strlen(command));
        if (strncmp(command, "exit", 4) == 0) {
            printf("Exiting...\n");
            break;
        }
    }
    printf("Exit...\n");
    munmap(map, FILE_SIZE);
    close(fd);

    return 0;
}