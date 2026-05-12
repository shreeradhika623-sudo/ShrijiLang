/*=============================================================
  SHRIJI GYAAN ENGINE — CLEAN (SILENT DATA LAYER)
=============================================================*/

#include <string.h>
#include "../../include/error.h"

/*──────────────────────────────────────────────
  RULE STRUCTURE
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

/* external lookup */
extern const GyaanRule* gyaan_lookup(int code);

/*──────────────────────────────────────────────
  SAFE STRING
──────────────────────────────────────────────*/

static const char* safe(const char *s)
{
    return (s && *s) ? s : NULL;
}

/*──────────────────────────────────────────────
  MAIN GYAAN ACCESS (NO PRINT)
──────────────────────────────────────────────*/

/*
    Purpose:
    - Return explanation text (core learning)
    - NO printing
    - KRST will decide how to use this
*/

const char* gyaan_get(const ShrijiErrorInfo *err)
{
    if (!err) return NULL;

    const GyaanRule *r = gyaan_lookup(err->code);

    /* fallback (no rule found) */
    if (!r) {
        return "Input ka structure sahi nahi hai";
    }

    /* priority: explain → rule → hint */
    if (safe(r->explain)) return r->explain;
    if (safe(r->rule))    return r->rule;
    if (safe(r->hint))    return r->hint;

    return NULL;
}

/*──────────────────────────────────────────────
  OPTIONAL: ADVANCED ACCESS (FUTURE USE)
──────────────────────────────────────────────*/

const char* gyaan_get_hint(const ShrijiErrorInfo *err)
{
    if (!err) return NULL;

    const GyaanRule *r = gyaan_lookup(err->code);
    if (!r) return NULL;

    return safe(r->hint);
}

const char* gyaan_get_rule(const ShrijiErrorInfo *err)
{
    if (!err) return NULL;

    const GyaanRule *r = gyaan_lookup(err->code);
    if (!r) return NULL;

    return safe(r->rule);
}
