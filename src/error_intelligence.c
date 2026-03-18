#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "../include/krst_router.h"
#include "../include/error.h"
#include "../include/pragya_avastha.h"

/* PREFILL SYSTEM */
static char *prefill_text = NULL;

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

/* POINTER */
static void print_pointer_multi(const char *text, int start)
{
    if (!text || start < 0) return;

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

/* OPERATOR TYPE */
static char detect_operator_type(const char *input)
{
    for (int i = 0; input[i]; i++)
    {
        if (strchr("+-*/", input[i]))
            return input[i];
    }
    return 0;
}

/* OPERATOR CHAIN */
static int operator_chain_at_position(const char *input, int pos, char op)
{
    int left = pos;
    int right = pos;
    int len = strlen(input);

    while (left >= 0 && input[left] == op) left--;
    while (right < len && input[right] == op) right++;

    return right - left - 1;
}

/* MULTI ERROR */
static int detect_all_operator_errors(const char *input, int positions[], char ops[], int max)
{
    int count = 0;

    for (int i = 0; input[i] && count < max; i++)
    {
        if (strchr("+-*/", input[i]) && input[i] == input[i + 1])
        {
            positions[count] = i;
            ops[count] = input[i];
            count++;
        }
    }

    return count;
}

/* 🔥 FIXED (NO STATIC) */
int suggest_basic_fixes(const char *input, int pos, char *out_expr)
{
    int left = pos, right = pos, len = strlen(input);

    while (left >= 0 && !isdigit(input[left])) left--;
    int end_left = left;

    while (left >= 0 && isdigit(input[left])) left--;
    left++;

    while (right < len && !isdigit(input[right])) right++;
    int start_right = right;

    while (right < len && isdigit(input[right])) right++;

    char left_num[64] = {0};
    char right_num[64] = {0};

    snprintf(left_num, sizeof(left_num), "%.*s", end_left - left + 1, input + left);
    snprintf(right_num, sizeof(right_num), "%.*s", right - start_right, input + start_right);

    if (!*left_num || !*right_num) return 0;

    printf("\nPossible interpretations:\n\n");
    printf("A) %s + %s\n", left_num, right_num);
    printf("B) %s - %s\n", left_num, right_num);
    printf("C) %s * %s\n", left_num, right_num);
    printf("D) %s / %s\n", left_num, right_num);
    printf("\nM) Manual fix\nI) Ignore\n");

    printf("\nSelect option: ");

    char line[32];

    while (fgets(line, sizeof(line), stdin))
    {
        char c = tolower(line[0]);

        if (c=='a'){sprintf(out_expr,"%s + %s",left_num,right_num);return 1;}
        if (c=='b'){sprintf(out_expr,"%s - %s",left_num,right_num);return 1;}
        if (c=='c'){sprintf(out_expr,"%s * %s",left_num,right_num);return 1;}
        if (c=='d'){sprintf(out_expr,"%s / %s",left_num,right_num);return 1;}

        if (c=='m')
        {
            printf("Enter expression: ");
            if (!fgets(out_expr,128,stdin)) return 0;
            out_expr[strcspn(out_expr,"\n")] = 0;
            return 1;
        }

        if (c=='i') return 0;

        printf("Invalid choice. Try again: ");
    }

    return 0;
}

/* MAIN ENGINE */
void shriji_error_intelligence(
    PragyaAvastha *avastha,
    const ShrijiErrorInfo *err,
    void *niyu_result)
{
    (void)niyu_result;

    if (!avastha || !err || !avastha->raw_text) return;

    const char *text = avastha->raw_text;
    int pos = err->col > 0 ? err->col - 1 : 0;

    int positions[10];
    char ops[10];

    int total = detect_all_operator_errors(text, positions, ops, 10);

    if (total > 1)
    {
        printf("\nMultiple operator errors detected:\n\n");

        for (int i = 0; i < total; i++)
            printf("%d) '%c%c' at position %d\n",
                   i+1, ops[i], ops[i], positions[i]+1);

        printf("\nE) Edit full expression\nI) Ignore\nSelect: ");

        char line[32];
        if (!fgets(line,sizeof(line),stdin)) return;

        if (tolower(line[0])=='e')
        {
            printf("\nEdit:\n%s\n", text);

            prefill_text = (char*)text;
            rl_startup_hook = prefill_hook;

            char *edited = readline("> ");
            rl_startup_hook = NULL;

            if (!edited || !*edited)
            {
                free(edited);
                return;
            }

            add_history(edited);

            avastha->has_correction = 1;
            avastha->stop_execution = 1;
            strcpy(avastha->corrected_text, edited);

            printf("\n[Shriji] updated → %s\n\n", edited);

            KRSTRequest req = { .input_text = edited };
            krst_route_request(&req);

            free(edited);
        }
        return;
    }

    char op = detect_operator_type(text);

    if (op)
    {
        int chain = operator_chain_at_position(text, pos, op);

        if (chain > 1)
        {
            printf("\n%d '%c' operators detected\n", chain, op);
            printf("Extra operator issue lag raha hai\n");
        }

        char fixed[128];

        if (suggest_basic_fixes(text, pos, fixed))
        {
            printf("\n[Shriji] auto fix → %s\n\n", fixed);

            KRSTRequest req = { .input_text = fixed };
            krst_route_request(&req);

            avastha->stop_execution = 1;
            return;
        }
    }

    print_pointer_multi(text, pos);
}
