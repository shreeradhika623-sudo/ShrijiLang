#ifndef NIYU_CONTRACT_H
#define NIYU_CONTRACT_H

#include "../../include/pragya_avastha.h"

/* =============================== */
/* NIYU ERROR TYPES                */
/* =============================== */

typedef enum {

    NIYU_ERR_VALID = 0,
    NIYU_ERR_SYNTAX,
    NIYU_ERR_MISSING_OPERAND,
    NIYU_ERR_EXTRA_OPERATOR,
    NIYU_ERR_INVALID_IDENTIFIER,
    NIYU_ERR_UNDEFINED_VARIABLE,
    NIYU_ERR_TYPE_MISMATCH,
    NIYU_ERR_DIV_ZERO,
    NIYU_ERR_UNEXPECTED_TOKEN,
    NIYU_ERR_EMPTY_INPUT,
    NIYU_ERR_INVALID_COMMAND,
    NIYU_ERR_RUNTIME

} NiyuErrorType;


/* =============================== */
/* NIYU INTENT TYPES               */
/* =============================== */

typedef enum {

    NIYU_INTENT_CALCULATION = 0,
    NIYU_INTENT_DECLARATION,
    NIYU_INTENT_PRINT,
    NIYU_INTENT_UNKNOWN

} NiyuIntent;


/* =============================== */
/* NIYU RESULT STRUCT              */
/* =============================== */

typedef struct {

    NiyuIntent intent;
    NiyuErrorType error_type;

    int severity;

    int error_pos;

} NiyuResult;


/* =============================== */
/* NIYU CORE API                   */
/* =============================== */

NiyuResult* niyu_think_avastha(PragyaAvastha *avastha);

void niyu_free_result(NiyuResult *res);

#endif
