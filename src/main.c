/* SHREE_RADHIKA_RANI 🌸 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "../include/parser.h"
#include "../include/interpreter.h"
#include "../include/runtime.h"
#include "../include/error.h"
#include "../include/smriti_session.h"
#include "../include/krst_router.h"
#include "../include/env.h"
#include "../include/value.h"

/* GLOBAL ENV */
Env *GLOBAL_ENV = NULL;

/* INIT SYSTEM */
static void init_system()
{
    GLOBAL_ENV = new_env();
}

/* REPL MODE */
static void run_repl_mode()
{
    char buffer[1024];

    printf("──────────────────────────────────────────────\n");
    printf("        🌸 Shriji Shell 🌸\n");
    printf("──────────────────────────────────────────────\n");
    printf("Type 'exit' to quit\n\n");

    while (1)
    {
#ifndef _WIN32
        char *line = readline("Shiri> ");
        if (!line) break;
#else
        char line_local[1024];
        printf("Shiri> ");
        if (!fgets(line_local, sizeof(line_local), stdin))
            break;

        line_local[strcspn(line_local, "\n")] = 0;
        char *line = line_local;
#endif

        /* EXIT */
        if (strcmp(line, "exit") == 0)
        {
#ifndef _WIN32
            free(line);
#endif
            break;
        }

        /* COPY INPUT */
        snprintf(buffer, sizeof(buffer), "%s", line);

        /*  MULTILINE BLOCK SUPPORT */
        if (strchr(buffer, '{') && !strchr(buffer, '}')) {
            char extra[1024];

            while (1) {
#ifndef _WIN32
                char *next = readline("... ");
                if (!next) break;

                snprintf(extra, sizeof(extra), "%s", next);
                free(next);
#else
                printf("... ");
                if (!fgets(extra, sizeof(extra), stdin)) break;
                extra[strcspn(extra, "\n")] = 0;
#endif

                 size_t len = strlen(buffer);
size_t extra_len = strlen(extra);

/* space check */
if (len + 1 + extra_len < sizeof(buffer)) {
    strcat(buffer, "\n");
    strcat(buffer, extra);
}



                if (strchr(extra, '}'))
                    break;
            }
        }

#ifndef _WIN32
        free(line);
#endif

        if (buffer[0] == '\0')
            continue;

        /*  FULL INPUT HISTORY (FIXED) */
#ifndef _WIN32
        add_history(buffer);
#endif

        /* STORE INPUT */
        smriti_session_set_last_input(buffer);

/* KRST CONTROL LOOP */
int fix_attempt = 0;
const int MAX_FIX_ATTEMPT = 5;

/*  BLOCK INPUT → SKIP KRST LOOP */
if (!(strchr(buffer, '{') && strchr(buffer, '}')))
while (fix_attempt < MAX_FIX_ATTEMPT)
{
    KRSTRequest req;

    req.input_text = buffer;
    req.has_correction = 0;
    req.corrected_text[0] = '\0';

    krst_route_request(&req);

    if (!req.has_correction)
        break;

    snprintf(buffer, sizeof(buffer), "%s", req.corrected_text);
    fix_attempt++;
}


        /* FINAL EXECUTION */
        error_reported = 0;
        shriji_set_error_mode(ERROR_MODE_IMMEDIATE);

        ASTNode *tree = parse_program(buffer);

        if (!tree || error_reported)
            continue;

        ShrijiRuntime rt;
        runtime_init(&rt);

        Value result = eval(tree, GLOBAL_ENV, &rt);
        value_free(&result);
    }
}

/* FILE MODE */
static void run_file_mode(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if (!fp)
    {
        printf("File open failed: %s\n", filename);
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = (char *)malloc(size + 1);
    if (!buffer)
    {
        fclose(fp);
        return;
    }

size_t read_bytes = fread(buffer, 1, size, fp);
(void)read_bytes;

buffer[size] = '\0';
fclose(fp);

    smriti_session_set_last_input(buffer);

    error_reported = 0;
    shriji_set_error_mode(ERROR_MODE_IMMEDIATE);

    ASTNode *tree = parse_program(buffer);

    if (!tree || error_reported)
    {
        free(buffer);
        return;
    }

    ShrijiRuntime rt;
    runtime_init(&rt);

    Value result = eval(tree, GLOBAL_ENV, &rt);

    value_free(&result);
    free(buffer);
}

/* MAIN */
int main(int argc, char **argv)
{
    init_system();

    if (argc > 1)
        run_file_mode(argv[1]);
    else
        run_repl_mode();

    return 0;
}
