#ifndef SHRIJI_FIX_RULES_H
#define SHRIJI_FIX_RULES_H

#include "../include/error.h"

/*
    SHRIJILANG — FIX CATEGORY SYSTEM
    ---------------------------------
    Ye layer error code ko fix category me convert karegi.

    IMPORTANT:
    Fix engine directly error code check nahi karega.
    Pehle category milegi.
    Fir category ke basis pe rule apply hoga.
*/

/* ─────────────────────────────────────
   FIX CATEGORY ENUM
───────────────────────────────────── */

typedef enum {

    FIX_NONE = 0,              // No fix possible
    FIX_MISSING_VALUE,         // '=' ke baad value missing
    FIX_BAD_TOKEN,             // Unexpected / invalid token
    FIX_STRUCTURAL_SAFE        // Safe structural auto-correction

} FixCategory;


/* ─────────────────────────────────────
   CLASSIFICATION FUNCTION
───────────────────────────────────── */

/*
    Error code → Fix category mapping
*/
FixCategory shriji_classify_error(ShrijiErrorCode code);


#endif
