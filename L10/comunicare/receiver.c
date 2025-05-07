#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_SIZE 1024

int main() {
    int fd = shm_open("/send-recv-stream", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, FILE_SIZE);

    void *map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    memset(map, 0, FILE_SIZE);
    if (map == MAP_FAILED) 
        exit(EXIT_FAILURE);

    int count = 0;
    int old_size = 0;
    int index = 0;
    long long buffer[10] = {0};
    printf("Monitor: Astept valori...\n");

    while (1) {
        int new_size = strlen((char*)map);
        if (new_size != old_size && new_size > 1) {
            old_size = new_size;
            if (count == 0) {
                index = 0;                
                count = atoi((char*)map);
                printf("Urmeaza sa receptionez %d valori QWORD\n", count);                
            }
            else {
                if (((char*)map)[16] == '!') {
                    char *endptr;
                    long long data = strtoull((char*)map, &endptr, 16);
                    printf("QWORD_%02d: %llX\n", index, data);
                    buffer[index++] = data;
                }
            }
        }
        else {
            if (((char*)map)[16] == '!') {
                char *endptr;
                long long data = strtoull((char*)map, &endptr, 16);
                printf("QWORD_%02d: %llX\n", index, data);
                buffer[index++] = data;
                if (index == count)
                {
                    memset(map, 0, FILE_SIZE);
                    msync(map, FILE_SIZE, MS_SYNC);            
                    break;
                }
            }
        }
        memset(map, 0, FILE_SIZE);
        msync(map, FILE_SIZE, MS_SYNC);
    }

    munmap(map, FILE_SIZE);
    close(fd);

    return 0;
}