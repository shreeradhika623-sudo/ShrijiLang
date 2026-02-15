#include <stdio.h>
#include "../include/lexer.h"

extern void tokenize_all(const char *source);

int main() {
    const char *code =
        "mavi x = 10 + 20\n"
        "\"Radhe Shyam\"";

    tokenize_all(code);
    return 0;
}
