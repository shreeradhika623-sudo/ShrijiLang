#include <stdio.h>
#include "../include/interpreter.h"

int main() {
    const char *src = "mavi x = 10 + 20";
    int result = run_source(src);
    printf("Result = %d\n", result);
    return 0;
}

