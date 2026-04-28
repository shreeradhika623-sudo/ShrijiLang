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

/* 🌸 GYAAN CONTROL */
extern int GYAAN_ALREADY_PRINTED;

/* GLOBAL ENV */
Env *GLOBAL_ENV = NULL;

/* INIT SYSTEM */
static void init_system()
{
    GLOBAL_ENV = new_env();
}

/* =========================
   REPL MODE
========================= */
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

#ifndef _WIN32
        free(line);
#endif

        if (buffer[0] == '\0')
            continue;

#ifndef _WIN32
        add_history(buffer);
#endif

        smriti_session_set_last_input(buffer);

        /* =========================
           🚀 KRST ONLY PIPELINE
        ========================= */
        KRSTRequest req;

        req.input_text = buffer;
        req.has_correction = 0;
        req.corrected_text[0] = '\0';
        req.stop_execution = 0;

        krst_route_request(&req);

        /* 🔁 Auto-correction loop */
        if (req.has_correction)
        {
            snprintf(buffer, sizeof(buffer), "%s", req.corrected_text);
            continue;
        }

        /* ⛔ Stop execution if KRST says */
        if (req.stop_execution)
        {
            continue;
        }

        /* 🚫 NO manual execution here */
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

    /* ✅ READ FILE (FIXED) */
    size_t read_bytes = fread(buffer, 1, size, fp);
    buffer[read_bytes] = '\0';

    /* ✅ CLOSE AFTER READ */
    fclose(fp);

    smriti_session_set_last_input(buffer);

    /* 🚀 KRST ONLY */
    KRSTRequest req;

    req.input_text = buffer;
    req.has_correction = 0;
    req.corrected_text[0] = '\0';
    req.stop_execution = 0;

    krst_route_request(&req);

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
