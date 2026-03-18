#include "niyu_contract.h"
#include "../../include/pragya_avastha.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================= */
/* Helper functions              */
/* ============================= */

static int is_operator(char c)
{
    return (c=='+' || c=='-' || c=='*' || c=='/');
}

static int is_digit_char(char c)
{
    return isdigit((unsigned char)c);
}

static int starts_with(const char *text, const char *word)
{
    return strncmp(text, word, strlen(word)) == 0;
}

/* ============================= */
/* Skip spaces                   */
/* ============================= */

static int next_non_space(const char *t, int i)
{
    while (t[i] && isspace((unsigned char)t[i]))
        i++;

    return i;
}


/* ============================= */
/* NIYU THINK ENGINE             */
/* ============================= */

NiyuResult* niyu_think_avastha(PragyaAvastha *a)
{
    if (!a || !a->raw_text)
        return NULL;

    const char *t = a->raw_text;

    NiyuResult *res = malloc(sizeof(NiyuResult));

    if (!res)
        return NULL;

    res->intent = NIYU_INTENT_UNKNOWN;
    res->error_type = NIYU_ERR_VALID;
    res->severity = 0;
    res->error_pos = -1;

    int len = strlen(t);

    if (len == 0)
    {
        res->error_type = NIYU_ERR_EMPTY_INPUT;
        res->severity = 1;
        return res;
    }

    /* ============================= */
    /* INTENT DETECTION              */
    /* ============================= */

    if (starts_with(t, "mavi"))
    {
        res->intent = NIYU_INTENT_DECLARATION;
        return res;
    }

    if (starts_with(t, "bolo"))
    {
        res->intent = NIYU_INTENT_PRINT;
        return res;
    }

    res->intent = NIYU_INTENT_CALCULATION;

    /* ============================= */
    /* ERROR PATTERN DETECTION       */
    /* ============================= */

    for (int i = 0; i < len; i++)
    {
        int j = next_non_space(t, i+1);

        if (!t[j])
            break;

        char c1 = t[i];
        char c2 = t[j];

        /* operator operator */

        if (is_operator(c1) && is_operator(c2))
        {
            res->error_type = NIYU_ERR_EXTRA_OPERATOR;
            res->severity = 2;
            res->error_pos = j;
            return res;
        }

        /* digit digit (missing operator) */

        if (is_digit_char(c1) && is_digit_char(c2))
        {
            if (j > i+1)
            {
                res->error_type = NIYU_ERR_SYNTAX;
                res->severity = 2;
                res->error_pos = j;
                return res;
            }
        }
    }

    /* ============================= */
    /* Missing operand               */
    /* ============================= */

    int last = len-1;

    while (last >= 0 && isspace((unsigned char)t[last]))
        last--;

    if (last >= 0 && is_operator(t[last]))
    {
        res->error_type = NIYU_ERR_MISSING_OPERAND;
        res->severity = 2;
        res->error_pos = last;
        return res;
    }

    /* ============================= */
    /* Undefined variable heuristic  */
    /* ============================= */

    if (isalpha((unsigned char)t[0]))
    {
        res->error_type = NIYU_ERR_UNDEFINED_VARIABLE;
        res->severity = 3;
        res->error_pos = 0;
        return res;
    }

    return res;
}


/* ============================= */
/* FREE RESULT                   */
/* ============================= */

void niyu_free_result(NiyuResult *res)
{
    if (res)
        free(res);
}
