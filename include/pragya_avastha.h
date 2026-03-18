#ifndef PRAGYA_AVASTHA_H
#define PRAGYA_AVASTHA_H

#include "ast.h"
#include "error.h"
#include "value.h"

/*
   PragyaAvastha

   Shriji system ki vartamaan sthiti.
   Yeh packet KRST se Pragya tak jata hai
   aur fir Niyu analysis ke liye use karti hai.
*/

typedef struct {

    /* User input */
    const char *raw_text;

    /* 🔥 NEW: correction system */
    int has_correction;
    char corrected_text[256];
    int stop_execution;
    /* Parsed structure */
    ASTNode *ast;

    /* Error state */
    ShrijiErrorCode error_code;

    int error_pos;
    int error_len;

    /* KRST metrics */
    int confidence;
    int risk;

    int teach_level;
    int tone;

    /* Runtime result */
    Value runtime_result;

} PragyaAvastha;

#endif
