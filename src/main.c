/*=============================================================
  SHRIJILANG — MAIN ENTRY
  L3 FROZEN COMPLIANT
=============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/interpreter.h"
#include "../include/parser.h"
#include "../include/env.h"
#include "../include/ai_router.h"
#include "../include/smriti.h"
#include "../include/value.h"

/*=============================================================
  GLOBAL ENVIRONMENT
=============================================================*/
Env *GLOBAL_ENV;

/*=============================================================
  FILE READER (SCRIPT MODE)
=============================================================*/
static char *read_file(const char *path)
{
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

/*=============================================================
  SCRIPT MODE
=============================================================*/
static void run_script(const char *filename)
{
    char *source = read_file(filename);
    if (!source) {
        printf("⛔ Failed to read %s\n", filename);
        return;
    }

    /* AI Router hook (routing only) */
    ShrijiBridgePacket pkt;
    pkt.text   = source;
    pkt.lang   = SHRIJI_LANG_AUTO;
    pkt.source = SHRIJI_SRC_FILE;
    ai_router_process_packet(&pkt);

    ASTNode *root = parse_program(source);
    if (!root) {
        free(source);
        return;
    }

    eval(root, GLOBAL_ENV);

    ast_free(root);
    free(source);
}

/*=============================================================
  INTERACTIVE SHELL (REPL)
=============================================================*/
static void interactive_shell(void)
{
    char line[512];

    printf("──────────────────────────────────────────────\n");
    printf("        🌸 ShrijiLang Interactive Shell 🌸\n");
    printf("──────────────────────────────────────────────\n");
    printf("Type 'exit' to quit\n\n");

    while (1)
    {
        printf("Shiri> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "exit") == 0) {
            printf("Radhe Radhe 🌸\n");
            break;
        }

        if (line[0] == '\0')
            continue;

        /* AI Router hook (routing only) */
        ShrijiBridgePacket pkt;
        pkt.text   = line;
        pkt.lang   = SHRIJI_LANG_AUTO;
        pkt.source = SHRIJI_SRC_REPL;
        ai_router_process_packet(&pkt);

        ASTNode *root = parse_program(line);
        if (!root)
            continue;

        Value result = eval(root, GLOBAL_ENV);

        if (result.type == VAL_NUMBER)
            printf("%.*g\n", 12, result.number);
        else if (result.type == VAL_STRING)
            printf("%s\n", result.string ? result.string : "");

        value_free(&result);
        ast_free(root);
    }
}

/*=============================================================
  MAIN ENTRY
=============================================================*/
int main(int argc, char **argv)
{
    GLOBAL_ENV = new_env();
    smriti_init();

    if (argc > 1)
        run_script(argv[1]);
    else
        interactive_shell();

    return 0;
}
