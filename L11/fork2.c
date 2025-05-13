#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>

#define TARGET_VAL 0x69FABBD1
#define NUM_PROCESSES 8

int main() {
    struct timespec start, end;

    int *result = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (result == MAP_FAILED) {
        exit(1);
    }

    *result = -1; 

    int segment_size = 0xFFFFFFFF / NUM_PROCESSES;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();

        if (pid < 0)
            exit(1);

        if (pid == 0) {
            int start = i * segment_size;
            int end = (i == NUM_PROCESSES - 1) ? 0xFFFFFFFF : start + segment_size;

            for (int j = start; j < end; j++) 
            {
                if (j == TARGET_VAL) {
                    *result = j;
                    printf("Am gasit EU(%d): %X\n", getpid(), TARGET_VAL);
                    msync(result, sizeof(int), MS_SYNC);
                    exit(0);
                }
                else
                {
                    if (*result != -1)
                    {
                        printf("Altcineva a gasit!\n");
                        exit(0);
                    }
                }
            }

            exit(0);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) 
        wait(0);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time: %.3f sec\n", elapsed_time);

    munmap(result, sizeof(int));

    return 0;
}
