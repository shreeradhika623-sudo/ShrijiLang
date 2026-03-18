#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <readline/readline.h>

static char *prefill_text = NULL;

#include "../include/krst_router.h"
#include "../include/error.h"
#include "../include/pragya_avastha.h"

/* =======================================================
   PROTOTYPES
======================================================= */

char detect_operator_type(const char *input);
int operator_chain_at_position(const char *input, int pos, char op);
int suggest_basic_fixes(const char *input, int pos, char *out_expr);
int detect_all_operator_errors(const char *input, int positions[], char ops[], int max);

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
/* =======================================================
   POINTER DISPLAY
======================================================= */

static void print_pointer_multi(const char *text, int start)
{
    if (!text || start < 0)
        return;

    int len = strlen(text);

    printf("%s\n", text);

    for (int i = 0; i < start; i++)
        printf(" ");

    while (start < len && !isalnum(text[start]) && text[start] != ' ')
    {
        printf("^");
        start++;
    }

    printf("\n\n");
}

/* =======================================================
   ERROR INTELLIGENCE ENGINE
======================================================= */

void shriji_error_intelligence(
    PragyaAvastha *avastha,
    const ShrijiErrorInfo *err,
    void *niyu_result)
{
    (void)niyu_result;

    if (!avastha || !err)
        return;

    const char *text = avastha->raw_text;

    if (!text)
        return;

    int pos = err->col - 1;

    /* ================= MULTI ERROR ================= */

    int positions[10];
    char ops[10];

    int total_errors = detect_all_operator_errors(text, positions, ops, 10);

    if (total_errors > 1)
    {
        printf("\nMultiple errors detected:\n\n");

        for (int i = 0; i < total_errors; i++)
        {
            printf("%d) '%c%c' at position %d\n",
                   i + 1,
                   ops[i],
                   ops[i],
                   positions[i] + 1);
        }

        printf("\nSuggestion: Use manual edit for best correction.\n\n");

        printf("E) Edit full expression\n");
        printf("I) Ignore\n");

        printf("\nSelect option: ");

        char line[128];

        if (!fgets(line, sizeof(line), stdin))
            return;

        char choice = line[0];

        if (choice == 'E' || choice == 'e')
        {

printf("\nEdit this line:\n%s\n", text);

/* 🔥 REAL PREFILL */
prefill_text = (char *)text;
rl_startup_hook = prefill_hook;

char *edited = readline("> ");

rl_startup_hook = NULL;   // cleanup

if (!edited)
    return;

if (strlen(edited) == 0)
{
    printf("\n⚠ Empty input ignored.\n\n");
    free(edited);
    return;
}
            if (!edited)
                return;

            if (strlen(edited) == 0)
            {
                printf("\n⚠ Empty input ignored.\n\n");
                free(edited);
                return;
            }

            add_history(edited);

            avastha->has_correction = 1;
            avastha->stop_execution = 1;
            strcpy(avastha->corrected_text, edited);

            printf("\n[Shriji] expression updated → %s\n\n", edited);

            KRSTRequest req;
            req.input_text = edited;
            krst_route_request(&req);

            free(edited);
            return;
        }

        return;
    }

    /* ================= SINGLE ERROR ================= */

    char op = detect_operator_type(text);

    if (op && err->col > 0)
    {
        int chain = operator_chain_at_position(text, err->col - 1, op);

        printf("\n%d '%c' operators in sequence detected\n", chain, op);
        printf("Lagta hai extra operators aa gaye hain.\n\n");

        char new_expr[128];

        if (suggest_basic_fixes(text, err->col - 1, new_expr))
        {
            printf("\n[Shriji] expression updated → %s\n\n", new_expr);

            KRSTRequest req;
            req.input_text = new_expr;

            krst_route_request(&req);

            avastha->stop_execution = 1;
            return;
        }
    }

    /* ================= POINTER ================= */

    print_pointer_multi(text, pos);
}

/* =======================================================
   MULTI ERROR DETECTOR
======================================================= */

int detect_all_operator_errors(const char *input, int positions[], char ops[], int max)
{
    int count = 0;

    for (int i = 0; input[i] && count < max; i++)
    {
        if (input[i] == '+' || input[i] == '-' ||
            input[i] == '*' || input[i] == '/')
        {
            if (input[i] == input[i + 1])
            {
                positions[count] = i;
                ops[count] = input[i];
                count++;
            }
        }
    }

    return count;
}

/* =======================================================
   OPERATOR TYPE
======================================================= */

char detect_operator_type(const char *input)
{
    for (int i = 0; input[i]; i++)
    {
        if (input[i] == '+' ||
            input[i] == '-' ||
            input[i] == '*' ||
            input[i] == '/')
        {
            return input[i];
        }
    }

    return 0;
}

/* =======================================================
   OPERATOR CHAIN
======================================================= */

int operator_chain_at_position(const char *input, int pos, char op)
{
    int left = pos;
    int right = pos;

    int len = strlen(input);

    while (left >= 0 && input[left] == op)
        left--;

    while (right < len && input[right] == op)
        right++;

    return right - left - 1;
}

/* =======================================================
   BASIC FIX ENGINE
======================================================= */

int suggest_basic_fixes(const char *input, int pos, char *out_expr)
{
    int left = pos;
    int right = pos;

    int len = strlen(input);

    while (left >= 0 && !isdigit(input[left]))
        left--;

    int end_left = left;

    while (left >= 0 && isdigit(input[left]))
        left--;

    left++;

    while (right < len && !isdigit(input[right]))
        right++;

    int start_right = right;

    while (right < len && isdigit(input[right]))
        right++;

    char left_num[64] = {0};
    char right_num[64] = {0};

    snprintf(left_num, sizeof(left_num), "%.*s",
             end_left - left + 1, input + left);

    snprintf(right_num, sizeof(right_num), "%.*s",
             right - start_right, input + start_right);

    if (strlen(left_num) == 0 || strlen(right_num) == 0)
        return 0;

    printf("\nPossible interpretations:\n\n");

    printf("A) %s + %s\n", left_num, right_num);
    printf("B) %s - %s\n", left_num, right_num);
    printf("C) %s * %s\n", left_num, right_num);
    printf("D) %s / %s\n", left_num, right_num);

    printf("\nM) Manual fix");
    printf("\nI) Ignore\n");

    printf("\nSelect option: ");

    char line[128];

    while (1)
    {
        if (!fgets(line, sizeof(line), stdin))
            return 0;

        char choice = line[0];

        if (choice == 'A' || choice == 'a')
        {
            snprintf(out_expr, 128, "%s + %s", left_num, right_num);
            return 1;
        }

        if (choice == 'B' || choice == 'b')
        {
            snprintf(out_expr, 128, "%s - %s", left_num, right_num);
            return 1;
        }

        if (choice == 'C' || choice == 'c')
        {
            snprintf(out_expr, 128, "%s * %s", left_num, right_num);
            return 1;
        }

        if (choice == 'D' || choice == 'd')
        {
            snprintf(out_expr, 128, "%s / %s", left_num, right_num);
            return 1;
        }

        if (choice == 'M' || choice == 'm')
        {
            printf("Type corrected expression: ");

            if (!fgets(out_expr, 128, stdin))
                return 0;

            out_expr[strcspn(out_expr, "\n")] = 0;

            return 1;
        }

        if (choice == 'I' || choice == 'i')
            return 0;

        printf("\nWrong option 🙂 Try again.\n");
        printf("Select option: ");
    }
}
