#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/interpreter.h"
#include "../include/parser.h"
#include "../include/env.h"

// Global Environment
Env *GLOBAL_ENV;

/* Read entire file into memory */
static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("File open failed");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buffer = (char *)malloc(size + 1);
    if (!buffer) {
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);
    return buffer;
}

static void run_script(const char *filename) {
    char *source = read_file(filename);
    if (!source) {
        printf("⛔ Failed to read %s\n", filename);
        return;
    }

    ASTNode *root = parse_program(source);
    if (!root) {
        free(source);
        return;
    }

    eval(root, GLOBAL_ENV);

    ast_free(root);
    free(source);
}

static void interactive_shell() {
    char line[512];

    printf("──────────────────────────────────────────────\n");
    printf("        🌸 ShrijiLang Interactive Shell 🌸\n");
    printf("──────────────────────────────────────────────\n");
    printf("Type 'exit' to quit\n\n");

    while (1) {
        printf("Shiri> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "exit") == 0) {
            printf("Radhe Radhe 🌸\n");
            break;
        }

        if (line[0] == '\0')
            continue;

        ASTNode *root = parse_program(line);
        if (!root)
            continue;

        eval(root, GLOBAL_ENV);
        ast_free(root);
    }
}

int main(int argc, char **argv) {
    GLOBAL_ENV = new_env();

    if (argc > 1) {
        run_script(argv[1]);
    } else {
        interactive_shell();
    }

    return 0;
}
