#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#define NUM_EXPRESSIONS 10

typedef struct {
    int a;
    int b;
    char op;
}Expression;

int compute(Expression expr) {
    switch (expr.op) {
        case '+': return expr.a + expr.b;
        case '-': return expr.a - expr.b;
        case '*': return expr.a * expr.b;
        default: return 0;
    }
}

int main() {
    int pipefd[2];

    if (pipe(pipefd) == -1) 
    {
        printf("pipe error");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) 
    {
        printf("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) 
    {
        close(pipefd[0]);
        srand(time(NULL));

        for (int i = 0; i < NUM_EXPRESSIONS; ++i) 
        {
            Expression expr;
            expr.a = rand() % 100;
            expr.b = rand() % 100;
            char ops[] = {'+', '-', '*'};
            expr.op = ops[rand() % 3];
            write(pipefd[1], &expr, sizeof(expr));
        }

        close(pipefd[1]);
        wait(NULL);      
    } 
    else 
    {
        close(pipefd[1]);

        Expression expr;
        ssize_t bytesRead;

        while ((bytesRead = read(pipefd[0], &expr, sizeof(expr))) > 0) 
        {
            int result = compute(expr);
            printf("Child: %d %c %d = %d\n", expr.a, expr.op, expr.b, result);
        }

        close(pipefd[0]);
    }

    return 0;
}