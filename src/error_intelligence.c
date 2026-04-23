#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef _WIN32
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "../include/error.h"
#include "../include/pragya_avastha.h"
#include "../include/fix_rules.h"

/* ===============================
   PREFILL (FOR EDIT MODE)
   =============================== */

static char safe_buffer[256];
static char *prefill_text = NULL;

#ifndef _WIN32
static int prefill_hook(void)
{
    if (prefill_text)
    {
        rl_insert_text(prefill_text);
        rl_point = strlen(prefill_text);
        prefill_text = NULL;
    }
    return 0;
}
#endif

/* ===============================
   USER INPUT (STRICT)
   =============================== */

static char get_user_choice()
{
#ifdef _WIN32
    char buffer[16];

    if (!fgets(buffer, sizeof(buffer), stdin))
        return 'i';

    char c = tolower(buffer[0]);

    if (c == 'e') return 'e';
    if (c == 'i') return 'i';

    return 'i';
#else
    char *line = readline("");

    if (!line || !*line)
    {
        free(line);
        return 'i';
    }

    char c = tolower(line[0]);
    free(line);

    if (c == 'e') return 'e';
    if (c == 'i') return 'i';

    return 'i';
#endif
}

/* ===============================
   POINTER DISPLAY
   =============================== */

static void print_pointer(const char *text, int pos)
{
    if (!text || pos < 0) return;

    printf("%s\n", text);

    for (int i = 0; i < pos; i++)
        printf(" ");

    printf("^\n\n");
}

/* ===============================
   BASIC VALIDATION
   =============================== */

static int contains_invalid_token(const char *input)
{
    for (int i = 0; input[i]; i++)
    {
        char c = input[i];

        if (isalnum(c) || isspace(c) || strchr("+-*/()._{}[]=<>!\"'", c))
            continue;

        return 1;
    }
    return 0;
}

static int has_unmatched_brackets(const char *input)
{
    int count = 0;

    for (int i = 0; input[i]; i++)
    {
        if (input[i] == '(') count++;
        else if (input[i] == ')') count--;
    }

    return count != 0;
}

static int is_valid_input(const char *input)
{
    if (!input || !*input) return 0;
    if (contains_invalid_token(input)) return 0;
    if (has_unmatched_brackets(input)) return 0;
    return 1;
}

/* ===============================
   SIMPLE OPERATOR CLEAN FIX
   =============================== */

static int filter_operators(const char *input, char *output)
{
    int i, j = 0;
    int last_was_op = 0;

    for (i = 0; input[i]; i++)
    {
        char c = input[i];

        if (strchr("+-*/", c))
        {
            if (last_was_op) continue;

            output[j++] = c;
            last_was_op = 1;
        }
        else
        {
            output[j++] = c;
            last_was_op = 0;
        }
    }

    output[j] = '\0';
    return strcmp(input, output) != 0;
}

/* ===============================
   MAIN ERROR INTELLIGENCE
   =============================== */

void shriji_error_intelligence(
    PragyaAvastha *avastha,
    const ShrijiErrorInfo *err,
    void *niyu_result)
{
    (void)niyu_result;

    if (!avastha || !err || !avastha->raw_text)
        return;

    const char *text = avastha->raw_text;

    /* ===== VALIDATION ===== */

    if (!is_valid_input(text))
    {
        avastha->stop_execution = 1;

        int pos = (err && err->col > 0) ? err->col - 1 : 0;
        print_pointer(text, pos);
        return;
    }

    /* ===== RULE BASED FIX ===== */

    FixRule *rule = shriji_get_rule_for_error(err->code);

    if (rule && rule->apply_fix)
    {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s", text);

        if (rule->apply_fix(buffer, sizeof(buffer), err))
        {
            if (rule->safety == FIX_SAFE_DETERMINISTIC)
            {
                printf("\n[Auto Fix Applied]\n");
                printf("Reason: deterministic rule\n");
                printf("Result: %s\n\n", buffer);

                avastha->has_correction = 1;
                avastha->stop_execution = 1;

                snprintf(avastha->corrected_text,
                         sizeof(avastha->corrected_text),
                         "%s", buffer);

                return;
            }

            printf("\nSuggestion: %s\n", buffer);
            printf("\nEdit (E) / Ignore (I): ");

            char choice = get_user_choice();

            if (choice == 'e')
            {
                avastha->stop_execution = 1;

                printf("\nEdit:\n%s\n", text);

#ifndef _WIN32
                snprintf(safe_buffer, sizeof(safe_buffer), "%s", text);
                prefill_text = safe_buffer;

                rl_startup_hook = prefill_hook;
#endif
                return;
            }

            avastha->stop_execution = 1;
            return;
        }
    }

    /* ===== SIMPLE FILTER FIX ===== */

    char filtered[256];

    if (filter_operators(text, filtered))
    {
        printf("\nSuggestion: %s\n", filtered);
        printf("\nEdit (E) / Ignore (I): ");

        char choice = get_user_choice();

        if (choice == 'e')
        {
            avastha->stop_execution = 1;

            printf("\nEdit:\n%s\n", text);

#ifndef _WIN32
            snprintf(safe_buffer, sizeof(safe_buffer), "%s", text);
            prefill_text = safe_buffer;

            rl_startup_hook = prefill_hook;
#endif
            return;
        }

        avastha->stop_execution = 1;
        return;
    }

    /* ===== FALLBACK POINTER ===== */

    int pos = (err && err->col > 0) ? err->col - 1 : 0;

    print_pointer(text, pos);

    avastha->stop_execution = 1;
}
