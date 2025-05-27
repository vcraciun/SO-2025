#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_PATH "/tmp/myfifo"

typedef struct {
    int a;
    int b;
    char op;
} Expression;

int compute(Expression expr) 
{
    switch (expr.op) 
    {
        case '+': return expr.a + expr.b;
        case '-': return expr.a - expr.b;
        case '*': return expr.a * expr.b;
        default: return 0;
    }
}

int main() {
    mkfifo(FIFO_PATH, 0666);

    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd < 0) 
    {
        printf("open error");
        exit(EXIT_FAILURE);
    }

    Expression expr;
    ssize_t bytesRead;

    while ((bytesRead = read(fd, &expr, sizeof(expr))) > 0) 
    {
        int result = compute(expr);
        printf("[Received]: %d %c %d = %d\n", expr.a, expr.op, expr.b, result);
    }

    close(fd);
    return 0;
}