/* SHREE_RADHIKA_RANI 🌸 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "../include/fix_engine.h"
#include "../include/env.h"
#include "../include/value.h"
#include "../include/krst_router.h"
#include "../include/pragya_avastha.h"
#include "../krst/krst_core.h"

/* GLOBAL ENV */
Env *GLOBAL_ENV = NULL;

/* INIT SYSTEM */
static void init_system()
{
    GLOBAL_ENV = new_env();
}

/* =========================
   EXECUTION (CLEAN PIPELINE)
========================= */
static void execute_input(char *input)
{
    if (!input || strlen(input) == 0)
        return;

    /* SECURITY: INPUT SIZE LIMIT */
    if (strlen(input) > 4096) {
        printf("❌ Input bahut bada hai\n");
        return;
    }

    /* KRST ENTRY (ONLY ENTRY POINT) */
    PragyaAvastha avastha = {0};
    avastha.raw_text = input;

    krst_process(&avastha);
}

/* =========================
   REPL MODE
========================= */
static void run_repl_mode()
{
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

        if (strcmp(line, "exit") == 0)
        {
#ifndef _WIN32
            free(line);
#endif
            printf("🌸 Exit Shriji 🌸\n");
            break;
        }

        if (strlen(line) == 0)
        {
#ifndef _WIN32
            free(line);
#endif
            continue;
        }

#ifndef _WIN32
        add_history(line);
#endif

        execute_input(line);

#ifndef _WIN32
        free(line);
#endif
    }
}

/* =========================
   FILE MODE
========================= */
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
    buffer[read_bytes] = '\0';

    fclose(fp);

    execute_input(buffer);

    free(buffer);
}

/* =========================
   MAIN
========================= */
int main(int argc, char **argv)
{
    init_system();

    if (argc > 1)
        run_file_mode(argv[1]);
    else
        run_repl_mode();

    return 0;
}

/* MR_MR. MANIKPURI */
