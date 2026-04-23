#include "../include/fix_rules.h"
#include "../include/fix_interactive.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*──────────────────────────────────────────────
    FIX FUNCTIONS
──────────────────────────────────────────────*/

/* FIX: double operator (6++6 → 6+6) */
static int fix_double_operator(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)buffer_size;
    (void)err;

    int changed = 0;

    for (int i = 0; buffer[i]; i++)
    {
        if ((buffer[i] == '+' || buffer[i] == '-' ||
             buffer[i] == '*' || buffer[i] == '/')
            && buffer[i] == buffer[i+1])
        {
            memmove(&buffer[i], &buffer[i+1], strlen(&buffer[i+1]) + 1);
            changed = 1;
            i--;
        }
    }

    return changed;
}

/* FIX: missing operator (6(7+7) → 6*(7+7)) */
static int fix_missing_operator(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)err;

    for (int i = 0; buffer[i]; i++)
    {
        if (isdigit(buffer[i]) && buffer[i+1] == '(')
        {
            if (strlen(buffer) + 2 >= buffer_size)
                return 0;

            memmove(&buffer[i+2], &buffer[i+1], strlen(&buffer[i+1]) + 1);
            buffer[i+1] = '*';

            return 1;
        }
    }

    return 0;
}

/* FIX: missing value after '=' */
static int fix_missing_value_after_equal(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)err;

    size_t len = strlen(buffer);

    if (len + 3 >= buffer_size)
        return 0;

    while (len > 0 && isspace((unsigned char)buffer[len - 1]))
        len--;

    if (len > 0 && buffer[len - 1] == '=')
    {
        strcat(buffer, " 0");
        return 1;
    }

    return 0;
}

/*──────────────────────────────────────────────
    FIX RULE TABLE
──────────────────────────────────────────────*/

static FixRule fix_rules[] = {

    /* deterministic: always correct */
    {
        .error_code = E_PARSE_MISSING_OPERATOR,
        .category = FIXCAT_MISSING_OPERATOR,
        .safety = FIX_SAFE_DETERMINISTIC,
        .confidence_penalty = 5,
        .max_attempt = 1,
        .reparse_required = 1,
        .apply_fix = fix_missing_operator
    },

    /* deterministic: assignment must have value */
    {
        .error_code = E_ASSIGN_02,
        .category = FIXCAT_MISSING_VALUE,
        .safety = FIX_SAFE_DETERMINISTIC,
        .confidence_penalty = 6,
        .max_attempt = 1,
        .reparse_required = 1,
        .apply_fix = fix_missing_value_after_equal
    },

    /* structural: operator cleanup (not always guaranteed) */
    {
        .error_code = E_PARSE_OPERATOR_CHAIN,
        .category = FIXCAT_EXTRA_OPERATOR,
        .safety = FIX_SAFE_STRUCTURAL,
        .confidence_penalty = 8,
        .max_attempt = 1,
        .reparse_required = 1,
        .apply_fix = fix_double_operator
    }

};

/*──────────────────────────────────────────────
    RULE LOOKUP
──────────────────────────────────────────────*/

static int fix_rule_count =
    sizeof(fix_rules) / sizeof(FixRule);

FixRule *shriji_get_rule_for_error(ShrijiErrorCode code)
{
    for (int i = 0; i < fix_rule_count; i++)
    {
        if (fix_rules[i].error_code == code)
            return &fix_rules[i];
    }

    return NULL;
}
