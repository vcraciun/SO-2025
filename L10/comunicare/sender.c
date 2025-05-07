#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_SIZE 1024

long long random_qword() {
    long long high = ((long long)rand() << 32) * 73;
    long long low  = (long long)rand();
    return high | low;
}

int main() {
    int fd = shm_open("/send-recv-stream", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, FILE_SIZE);

    void *map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Trimit date: ... \n");
    sprintf(map, "%d", 10);
    msync(map, FILE_SIZE, MS_SYNC);
    sleep(1);
    for (int i = 0; i < 10; i++) {
        long long qword = random_qword();
        sprintf(map, "%016llX!", qword);        
        msync(map, FILE_SIZE, MS_SYNC);
        printf("Am scris: %016llX\n", qword);
        while (((char*)map)[0] != 0)
            ;
        sleep(1);
    }

    munmap(map, FILE_SIZE);
    close(fd);

    return 0;
}