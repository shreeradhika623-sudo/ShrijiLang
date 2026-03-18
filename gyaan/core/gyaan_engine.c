/*=============================================================
  SHRIJI GYAAN ENGINE — CORE (FINAL SMART VERSION)
=============================================================*/

#include <stdio.h>
#include <string.h>
#include "../../include/error.h"

/*──────────────────────────────────────────────
  STRUCT (same as rules)
──────────────────────────────────────────────*/
typedef struct {
    int code;

    const char *module;
    const char *file;
    const char *function;

    const char *rule;
    const char *explain;
    const char *hint;

} GyaanRule;

/*──────────────────────────────────────────────
  EXTERNAL LOOKUP
──────────────────────────────────────────────*/
extern const GyaanRule* gyaan_lookup(int code);

/*──────────────────────────────────────────────
  INTERNAL HELPERS
──────────────────────────────────────────────*/

/* safe print (null-safe) */
static const char* safe(const char *s)
{
    return (s && *s) ? s : "N/A";
}

/*──────────────────────────────────────────────
  MAIN PRINT ENGINE
──────────────────────────────────────────────*/
void gyaan_print(const ShrijiErrorInfo *err)
{
    if (!err) return;

    const GyaanRule *r = gyaan_lookup(err->code);

    /*──────────────────────────────────────────────
      FALLBACK (no rule found)
    ──────────────────────────────────────────────*/
    if (!r) {
        printf("\n🧠 SHRIJI INSIGHT\n");
        printf("──────────────────────────────\n");

        printf("Unknown error insight\n");
        printf("Explain  : system ko is error ke liye gyaan nahi mila\n");
        printf("Hint     : syntax ya logic manually check karein\n");

        printf("──────────────────────────────\n");
        return;
    }

    /*──────────────────────────────────────────────
      NORMAL RULE PRINT
    ──────────────────────────────────────────────*/
    printf("\n🧠 SHRIJI INSIGHT\n");
    printf("──────────────────────────────\n");

    printf("Module   : %s\n", safe(r->module));
    printf("File     : %s\n", safe(r->file));
    printf("Function : %s\n\n", safe(r->function));

    printf("Rule     : %s\n", safe(r->rule));
    printf("Explain  : %s\n", safe(r->explain));
    printf("Hint     : %s\n", safe(r->hint));

    /*──────────────────────────────────────────────
      EXTRA CONTEXT (future ready)
    ──────────────────────────────────────────────*/
    if (err->has_location) {
        printf("\nLocation : line %d, col %d\n", err->line, err->col);
    }

    printf("──────────────────────────────\n");
}
