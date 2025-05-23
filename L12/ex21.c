#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char *argv[] = {"ex21.sh", NULL};
    const char *default_path = getenv("PATH");
    char new_path[1024] = {0};
    snprintf(new_path, sizeof(new_path), "PATH=.:%.900s", default_path ? default_path : "");
    char *envp[] = {
        "VAR_FROM_PARENT=SO-2025",
        new_path,
        NULL
    };
    execvpe("./ex21.sh", argv, envp);
    printf("Something went wrong!\n");
    return 1;
}
