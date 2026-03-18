// SHRIJI NIYAM

#include "../include/fix_rules.h"
#include "../include/fix_interactive.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*──────────────────────────────────────────────
    Helper Utilities
──────────────────────────────────────────────*/

static int append_char(char *buffer, size_t size, char c)
{
    size_t len = strlen(buffer);

    if (len + 2 >= size)
        return 0;

    buffer[len] = c;
    buffer[len + 1] = '\0';

    return 1;
}

static int remove_trailing_comma(char *buffer)
{
    int len = strlen(buffer);

    if (len < 2)
        return 0;

    if (buffer[len-2] == ',' &&
       (buffer[len-1] == ']' || buffer[len-1] == ')'))
    {
        memmove(&buffer[len-2], &buffer[len-1], 2);
        return 1;
    }

    return 0;
}

/*──────────────────────────────────────────────
    Deterministic Fixes
──────────────────────────────────────────────*/

/* a =  → a = 0 */

static int fix_missing_value_after_equal(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)err;

    if (strlen(buffer) + 3 >= buffer_size)
        return 0;

    strcat(buffer, " 0");

    return 1;
}

/* missing bracket */

static int fix_missing_right_bracket(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)err;
    return append_char(buffer, buffer_size, ')');
}

/* missing quote */
/*
static int fix_missing_quote(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)err;
    return append_char(buffer, buffer_size, '"');
} */


/* remove trailing comma */

static int fix_trailing_comma(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)buffer_size;
    (void)err;

    return remove_trailing_comma(buffer);
}

/*──────────────────────────────────────────────
    Interactive Fixes
──────────────────────────────────────────────*/

static int fix_missing_operator(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)err;

    char fixed[512];

    if (!shriji_interactive_operator_fix(
            buffer,
            fixed,
            sizeof(fixed)))
        return 0;

    strncpy(buffer, fixed, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';

    return 1;
}

static int fix_double_operator(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)err;

    char fixed[512];

    if (!shriji_interactive_double_operator_fix(
            buffer,
            fixed,
            sizeof(fixed)))
        return 0;

    strncpy(buffer, fixed, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';

    return 1;
}

/*──────────────────────────────────────────────
    Implicit Multiplication
    Example:
        4(5+6) → 4*(5+6)
        x(5+6) → x*(5+6)
──────────────────────────────────────────────*/

static int fix_implicit_multiplication(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err)
{
    (void)err;

    char fixed[512];
    int j = 0;

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if ((isalnum(buffer[i])) && buffer[i+1] == '(')
        {
            fixed[j++] = buffer[i];
            fixed[j++] = '*';
        }
        else if (buffer[i] == ')' && buffer[i+1] == '(')
        {
            fixed[j++] = ')';
            fixed[j++] = '*';
        }
        else
        {
            fixed[j++] = buffer[i];
        }
    }

    fixed[j] = '\0';

    strncpy(buffer, fixed, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';

    return 1;
}

/*──────────────────────────────────────────────
    Fix Rule Table
──────────────────────────────────────────────*/

static FixRule fix_rules[] = {

/* operator chain: 6+*9 */

{
    E_PARSE_OPERATOR_CHAIN,
    FIX_EXTRA_OPERATOR,
    FIX_SAFE_STRUCTURAL,
    5,
    1,
    1,
    fix_double_operator
},

/* double operator: 6++6 */

{
    E_PARSE_DOUBLE_OPERATOR,
    FIX_EXTRA_OPERATOR,
    FIX_SAFE_DETERMINISTIC,
    5,
    1,
    1,
    fix_double_operator
},

/* implicit multiplication */

{
    E_PARSE_MISSING_OPERATOR,
    FIX_BAD_TOKEN,
    FIX_SAFE_DETERMINISTIC,
    3,
    1,
    1,
    fix_implicit_multiplication
},

/* interactive operator fix */

{
    E_PARSE_MISSING_OPERATOR,
    FIX_MISSING_OPERATOR,
    FIX_SAFE_STRUCTURAL,
    5,
    1,
    1,
    fix_missing_operator
},

/* assignment value missing */

{
    E_ASSIGN_02,
    FIX_MISSING_VALUE,
    FIX_SAFE_DETERMINISTIC,
    10,
    1,
    1,
    fix_missing_value_after_equal
},

/* bracket missing */

{
    E_PARSE_BRACKET_MISSING,
    FIX_BAD_TOKEN,
    FIX_SAFE_DETERMINISTIC,
    4,
    1,
    1,
    fix_missing_right_bracket
},

/* trailing comma */

{
    E_LIST_SYNTAX_ERROR,
    FIX_BAD_TOKEN,
    FIX_SAFE_DETERMINISTIC,
    4,
    1,
    1,
    fix_trailing_comma
}

};

/*──────────────────────────────────────────────
    Rule Lookup
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
