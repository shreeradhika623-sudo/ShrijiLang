/* SHREE_RADHIKA_RANI */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/parser.h"
#include "../include/error.h"
#include "../include/fix_engine.h"
#include "../include/fix_rules.h"

#define MAX_WORKING_BUFFER 512


/*──────────────────────────────────────────────
   PARSE ONLY (STRICT MODE)
──────────────────────────────────────────────*/

ASTNode *parse_once(const char *input)
{
    error_reported = 0;

    /* silent mode (no duplicate print) */
    shriji_set_error_mode(ERROR_MODE_COLLECT);

    ASTNode *node = parse_program(input);

    /* restore normal mode */
    shriji_set_error_mode(ERROR_MODE_IMMEDIATE);

    return node;
}


/*──────────────────────────────────────────────
   MAIN EXECUTION (SAFE VERSION)
──────────────────────────────────────────────*/
ASTNode *language_execute_with_fix(
    const char *input,
    char *final_output,
    int *was_fixed,
    int *penalty_out
)

{
    if (!input)
        return NULL;

    if (was_fixed)
        *was_fixed = 0;

    if (penalty_out)
        *penalty_out = 0;

    error_reported = 0;
    shriji_set_error_mode(ERROR_MODE_IMMEDIATE);


ASTNode *root = parse_once(input);

/* 🔻 TRY LOOP (max 2 attempts) */
int attempts = 0;
char working[MAX_WORKING_BUFFER];

strncpy(working, input, sizeof(working) - 1);
working[sizeof(working) - 1] = '\0';

while ((!root || error_reported) && attempts < 2)
{
    char fixed[MAX_WORKING_BUFFER];
    memset(fixed, 0, sizeof(fixed));

    FixType type = fix_apply(working, fixed);

    if (type != FIX_SAFE)
        break;

    if (was_fixed)
        *was_fixed = 1;

    if (penalty_out)
        *penalty_out += 3;

    error_reported = 0;

    strncpy(working, fixed, sizeof(working));

    root = parse_once(working);

    attempts++;
}

/* FINAL CHECK */
if (!root || error_reported)
    return NULL;

if (was_fixed && final_output)
{
    strncpy(final_output, working, MAX_WORKING_BUFFER);
}

return root;

if (final_output)
{
    strncpy(final_output, working, MAX_WORKING_BUFFER - 1);
    final_output[MAX_WORKING_BUFFER - 1] = '\0';
}

}


/*──────────────────────────────────────────────
   HELPER: invalid token
──────────────────────────────────────────────*/

static int has_invalid_char(const char *input)
{
    for (int i = 0; input[i]; i++)
    {
        char c = input[i];

        if (isdigit(c) || strchr("+-*/(). ", c))
            continue;

        return 1;
    }
    return 0;
}


/*──────────────────────────────────────────────
   HELPER: operator chain length
──────────────────────────────────────────────*/

static int operator_chain_len(const char *input, int pos)
{
    int len = 1;

    while (input[pos + len] != '\0' &&
           strchr("+-*/", input[pos + len]))
    {
        len++;
    }

    return len;
}


/*──────────────────────────────────────────────
   MAIN FIX ENGINE (STRICT MODE)
──────────────────────────────────────────────*/

FixType fix_apply(const char *input, char *output)
{
    if (!input || !*input)
        return FIX_NONE;

    if (has_invalid_char(input))
        return FIX_REJECT;

    int i, j = 0;
    int changed = 0;

    for (i = 0; input[i]; i++)
    {
        /* implicit multiplication */
        if (isdigit(input[i]) && input[i+1] == '(')
        {
            output[j++] = input[i];
            output[j++] = '*';
            changed = 1;
            continue;
        }

        if (input[i] == ')' && isdigit(input[i+1]))
        {
            output[j++] = input[i];
            output[j++] = '*';
            changed = 1;
            continue;
        }

        /* STRICT: operator chain NOT allowed */
        if (strchr("+-*/", input[i]))
        {
            int len = operator_chain_len(input, i);

            if (len >= 2)
            {
                return FIX_DOUBTFUL;
            }
        }

        output[j++] = input[i];
    }

    output[j] = '\0';

    if (changed)
        return FIX_SAFE;

    return FIX_NONE;
}
