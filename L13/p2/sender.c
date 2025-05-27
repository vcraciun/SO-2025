#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_PATH "/tmp/myfifo"

typedef struct {
    int a;
    int b;
    char op;
} Expression;

int main() {
    mkfifo(FIFO_PATH, 0666);

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd < 0) 
    {
        printf("open");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (int i = 0; i < 10; ++i) 
    {
        Expression expr;
        expr.a = rand() % 100;
        expr.b = rand() % 100;
        char ops[] = {'+', '-', '*'};
        expr.op = ops[rand() % 3];

        write(fd, &expr, sizeof(expr));
        usleep(100000); 
    }

    close(fd);
    return 0;
}