#include <stdio.h>
#include "../include/parser.h"

int main() {
    const char *src =
        "mavi x = 10 + 20";

    ASTNode *root = parse_program(src);

    print_ast(root, 0);
    return 0;
}
