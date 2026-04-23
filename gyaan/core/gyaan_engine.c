/*=============================================================
  SHRIJI GYAAN ENGINE — CORE (HINGLISH + CLEAR UX VERSION)
=============================================================*/

#include <stdio.h>
#include <string.h>
#include "../../include/error.h"

typedef struct {
    int code;

    const char *module;
    const char *file;
    const char *function;

    const char *rule;
    const char *explain;
    const char *hint;

} GyaanRule;

extern const GyaanRule* gyaan_lookup(int code);

static const char* safe(const char *s)
{
    return (s && *s) ? s : "N/A";
}

void gyaan_print(const ShrijiErrorInfo *err)
{
    if (!err) return;

    const GyaanRule *r = gyaan_lookup(err->code);

    /* SOFT FUNCTION MATCH */
    if (r && err->function && r->function) {
        if (strcmp(err->function, r->function) != 0) {
            /* allow fallback */
        }
    }

    /* FALLBACK (NO RULE FOUND) */
    if (!r) {
        printf("\nSHRIJI INSIGHT\n");
        printf("──────────────────────────────\n");

        printf("System ko is error ka exact explanation nahi mila\n");

        printf("Samjho   : input ka structure (syntax) sahi format me nahi hai\n");

        printf("Hint     : statement ko proper format me likho\n");
        printf("Example  : bolo 5 + 2\n");

        if (err->has_location) {
            printf("\nLocation : line %d, col %d\n", err->line, err->col);
        }

        printf("──────────────────────────────\n");
        return;
    }

    /* NORMAL RULE PRINT */
    printf("\nSHRIJI INSIGHT\n");
    printf("──────────────────────────────\n");

    printf("Module   : %s\n", safe(r->module));
    printf("File     : %s\n", safe(r->file));
    printf("Function : %s\n\n", safe(r->function));

    printf("Rule     : %s\n", safe(r->rule));
    printf("Samjho   : %s\n", safe(r->explain));
    printf("Hint     : %s\n", safe(r->hint));

    switch (err->code)
    {
        case E_PARSE_MISSING_OPERATOR:
            printf("\nNote     : yahan values ke beech operator missing tha\n");
            break;

        case E_PARSE_DOUBLE_OPERATOR:
            printf("\nNote     : do operator ek saath aa gaye the\n");
            break;

        case E_PARSE_OPERATOR_START:
            printf("\nNote     : expression operator se start nahi hota\n");
            break;

        case E_PARSE_OPERATOR_END:
            printf("\nNote     : expression operator par khatam ho gaya\n");
            break;

        default:
            break;
    }

    if (err->has_location) {
        printf("\nLocation : line %d, col %d\n", err->line, err->col);
    }

    printf("──────────────────────────────\n");
}
