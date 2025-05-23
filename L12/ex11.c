#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *env = getenv("VAR_FROM_PARENT");
    printf("VAR_FROM_PARENT = %s\n", env ? env : "NOT AVAIL");
    return 0;
}

