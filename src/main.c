/*
──────────────────────────────────────────────
    SHRIJILANG — MAIN ENTRY (KRST Controlled)
──────────────────────────────────────────────
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "../include/env.h"
#include "../include/smriti.h"
#include "../krst/krst_core.h"

#include "../include/typo_engine.h"
#include "../include/krst_router.h"

/* GLOBAL ENV — visible to KRST */

Env *GLOBAL_ENV = NULL;

/*──────────────────────────────────────────────
  INIT SYSTEM
──────────────────────────────────────────────*/

static void shriji_init(void)
{
    GLOBAL_ENV = new_env();
    smriti_init();
}

/*──────────────────────────────────────────────
  RUN REPL MODE
──────────────────────────────────────────────*/

static void run_repl_mode(void)
{
    printf("──────────────────────────────────────────────\n");
    printf("        🌸 ShrijiLang + KRST Shell 🌸\n");
    printf("──────────────────────────────────────────────\n");
    printf("Type 'exit' to quit\n\n");

    while (1)
    {
        /* 🔥 READLINE INPUT */
        char *line = readline("Shiri> ");

        if (!line)
            break;

        if (*line)
            add_history(line);

        /* EXIT */
        if (strcmp(line, "exit") == 0)
        {
            printf("Radhe Radhe 🌸\n");
            free(line);
            break;
        }

        /* EMPTY INPUT */
        if (line[0] == '\0')
        {
            free(line);
            continue;
        }

        /* TYPO CHECK */
        char suggestion[32];

        if (shriji_check_typo(line, suggestion, sizeof(suggestion)))
        {
            printf("Kya aapka matlab \"%s\" tha ?\n", suggestion);
            printf("(A) Apply / (Any key) Ignore : ");

            char choice = getchar();
            getchar(); // consume newline

            if (choice == 'A' || choice == 'a')
            {
                strcpy(line, suggestion);
                printf("[SYSTEM] Typo corrected.\n");
            }
            else
            {
                printf("[SYSTEM] Suggestion ignored.\n");
            }
        }

        /* EXIT AGAIN AFTER TYPO */
        if (strcmp(line, "exit") == 0)
        {
            printf("Radhe Radhe 🌸\n");
            free(line);
            break;
        }

        /* KRST REQUEST */
        KRSTRequest req;

        req.input_text = line;
        req.has_correction = 0;

        krst_route_request(&req);

        /* IF CORRECTED EXPRESSION RETURNED */
        if (req.has_correction)
        {
            strcpy(line, req.corrected_text);

            printf("\n[Shriji] Re-running corrected expression...\n\n");
            printf("Shiri> %s\n", line);

            req.input_text = line;
            req.has_correction = 0;

            krst_route_request(&req);
        }

        /* 🔥 IMPORTANT: free memory */
        free(line);
    }
}

/*──────────────────────────────────────────────
  MAIN ENTRY
──────────────────────────────────────────────*/

int main(void)
{
    shriji_init();
    run_repl_mode();
    return 0;
}
