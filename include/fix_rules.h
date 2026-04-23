#ifndef SHRIJI_FIX_RULES_H
#define SHRIJI_FIX_RULES_H

#include <stddef.h>
#include "error.h"

/* ─────────────────────────────────────────────
   FIX CATEGORY
───────────────────────────────────────────── */

typedef enum {
    FIXCAT_NONE = 0,
    FIXCAT_MISSING_VALUE,
    FIXCAT_BAD_TOKEN,
    FIXCAT_MISSING_OPERATOR,
    FIXCAT_MISSING_IDENTIFIER,
    FIXCAT_EXTRA_OPERATOR
} FixCategory;

/* ─────────────────────────────────────────────
   FIX SAFETY LEVEL
───────────────────────────────────────────── */

typedef enum {
    FIX_SAFE_DETERMINISTIC = 0,   /* 100% sure → auto apply */
    FIX_SAFE_STRUCTURAL,          /* safe but not guaranteed */
    FIX_RISKY_CONTEXTUAL          /* user decision required */
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

    FixApplyFunction apply_fix;

} FixRule;

/* ─────────────────────────────────────────────
   RULE LOOKUP
───────────────────────────────────────────── */

FixRule *shriji_get_rule_for_error(ShrijiErrorCode code);

#endif
