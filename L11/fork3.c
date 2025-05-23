#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdatomic.h>
#include <time.h>

#define N 5000000000
#define PROCESE 2

atomic_flag lock = ATOMIC_FLAG_INIT;

void enter_lock() {
    while (atomic_flag_test_and_set(&lock)) 
        ;
}

void leave_lock() {
    atomic_flag_clear(&lock);
}

int main() {
    struct timespec tstart, tend;

    int *shared_data = mmap(NULL, 3 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_data == MAP_FAILED) 
        exit(EXIT_FAILURE);

    unsigned long long *total_sum = (unsigned long long*)&shared_data[0];
    int *lock = &shared_data[2];

    *total_sum = 0;
    *lock = 0;

    long long range = N / PROCESE;
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (int i = 0; i < PROCESE; i++) {
        pid_t pid = fork();

        if (pid < 0)
            exit(EXIT_FAILURE);

        if (pid == 0) {
            long long start = i * range + 1;
            long long end = (i == PROCESE - 1) ? N : start + range - 1;

            long long local_sum = 0;
            for (long long j = start; j <= end; j++)
            {
                //local_sum += j;
                if (i%2 == 0)
                    local_sum += 1;
                else
                    local_sum -= 1;
            }

            enter_lock();
            *total_sum += local_sum;
            leave_lock();

            exit(0);
        }
    }

    for (int i = 0; i < PROCESE; i++) 
        wait(NULL);

    clock_gettime(CLOCK_MONOTONIC, &tend);
    double elapsed_time = (tend.tv_sec - tstart.tv_sec) + (tend.tv_nsec - tstart.tv_nsec) / 1000000000.0;    

    printf("Suma primelor %lu numere este: %llu, timp: %.3f\n", N, *total_sum, elapsed_time);

    munmap(shared_data, 2 * sizeof(int));
    return 0;
}
