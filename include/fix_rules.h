#ifndef SHRIJI_FIX_RULES_H
#define SHRIJI_FIX_RULES_H

#include <stddef.h>
#include "error.h"
/* ─────────────────────────────────────────────
   FIX CATEGORY
───────────────────────────────────────────── */

typedef enum {
    FIX_NONE = 0,
    FIX_MISSING_VALUE,
    FIX_BAD_TOKEN,
    FIX_MISSING_OPERATOR,
    FIX_MISSING_IDENTIFIER,
    FIX_EXTRA_OPERATOR,
} FixCategory;

/* ─────────────────────────────────────────────
   FIX SAFETY LEVEL
───────────────────────────────────────────── */

typedef enum {
    FIX_SAFE_DETERMINISTIC = 0,
    FIX_SAFE_STRUCTURAL,
    FIX_RISKY_CONTEXTUAL
} FixSafetyLevel;

/* ─────────────────────────────────────────────
   FIX FUNCTION SIGNATURE
───────────────────────────────────────────── */

typedef int (*FixApplyFunction)(
    char *buffer,
    size_t buffer_size,
    const ShrijiErrorInfo *err
);

/* ─────────────────────────────────────────────
   FIX RULE STRUCTURE
───────────────────────────────────────────── */

typedef struct {

    ShrijiErrorCode error_code;
    FixCategory category;
    FixSafetyLevel safety;

    int confidence_penalty;
    int max_attempt;
    int reparse_required;

    /* 🔥 functional rule */
    FixApplyFunction apply_fix;

} FixRule;

/* ─────────────────────────────────────────────
   RULE LOOKUP
───────────────────────────────────────────── */

FixRule *shriji_get_rule_for_error(ShrijiErrorCode code);

#endif
