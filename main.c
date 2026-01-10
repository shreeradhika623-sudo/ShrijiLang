#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/ast.h"
#include "include/parser.h"
#include "include/interpreter.h"
#include "include/env.h"
#include "include/error.h"
#include "shiri/shiri.h"

/*──────────────────────────────────────────────────────────────
 | SHRIJILANG — MAIN ENTRY
 |
 | Modes:
 |   1) REPL        → ./shrijilang
 |   2) SCRIPT RUN  → ./shrijilang file.sri
 |   3) SCRIPT RUN  → ./shrijilang run file.sri   (legacy)
 |
 | Core rule:
 |   • .sri file is parsed as ONE PROGRAM (AST_PROGRAM)
 |   • No line-by-line execution
 *──────────────────────────────────────────────────────────────*/

#define INPUT_MAX 1024

/*──────────────────────────────────────────────────────────────
 | Banner
 *──────────────────────────────────────────────────────────────*/
static void print_banner(void) {
    printf("──────────────────────────────────────────────\n");
    printf("        ShrijiLang — Universe-Class V1\n");
    printf("   (Sakhi + Niyu + Shiri + Mira + Kavya AI)\n");
    printf("──────────────────────────────────────────────\n");
}

/*──────────────────────────────────────────────────────────────
 | Read whole file dynamically (NO size limit)
 *──────────────────────────────────────────────────────────────*/
static char *read_entire_file(const char *filename) {

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Unable to open .sri file");
        return NULL;
    }

    size_t cap = 8192;
    size_t len = 0;
    char *buf = (char *)malloc(cap);
    if (!buf) {
        fclose(fp);
        return NULL;
    }

    int ch;
    while ((ch = fgetc(fp)) != EOF) {

        if (len + 2 >= cap) {
            cap *= 2;
            char *nbuf = (char *)realloc(buf, cap);
            if (!nbuf) {
                free(buf);
                fclose(fp);
                return NULL;
            }
            buf = nbuf;
        }

        buf[len++] = (char)ch;
    }

    buf[len] = '\0';
    fclose(fp);
    return buf;
}

/*──────────────────────────────────────────────────────────────
 | Run .sri script as FULL PROGRAM  ✅ FIXED (no script limit)
 *──────────────────────────────────────────────────────────────*/
static int run_script(const char *filename) {

    char *source = read_entire_file(filename);
    if (!source)
        return 1;

    Env *env = new_env();
    if (!env) {
        free(source);
        return 1;
    }

    ASTNode *program = parse_program(source);
    free(source);

    if (!program) {
        /* parser already reported error */
        return 1;
    }

    run_program(program, env);

    ast_free(program);
    return 0;
}

/*──────────────────────────────────────────────────────────────
 | Interactive REPL
 *──────────────────────────────────────────────────────────────*/
static void run_repl(void) {

    Env *env = new_env();
    if (!env)
        return;

    char line[INPUT_MAX];

    while (1) {

        printf("shiri >>> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "exit") == 0)
            break;

        if (line[0] == '\0')
            continue;

        ASTNode *node = parse_program(line);
        if (!node)
            continue;

        long long result = eval(node, env);

        /* Pure expression output (not assignment / identifier / command) */
        if (node->type != AST_IDENTIFIER &&
            node->type != AST_ASSIGNMENT &&
            node->type != AST_COMMAND &&
            node->type != AST_UPDATE) {

            char msg[128];
            snprintf(msg, sizeof(msg),
                     "Calculation result is %lld", result);

            const char *out = shiri_process(msg);
            if (out && *out)
                printf("%s\n", out);
        }

        ast_free(node);
    }
}

/*──────────────────────────────────────────────────────────────
 | MAIN
 *──────────────────────────────────────────────────────────────*/
int main(int argc, char *argv[]) {

    print_banner();

    /* 1) ./shrijilang file.sri */
    if (argc == 2) {
        return run_script(argv[1]);
    }

    /* 2) ./shrijilang run file.sri (legacy) */
    if (argc == 3 && strcmp(argv[1], "run") == 0) {
        return run_script(argv[2]);
    }

    /* 3) REPL mode */
    run_repl();

    printf("\nShrijiLang session ended.\n");
    return 0;
}
