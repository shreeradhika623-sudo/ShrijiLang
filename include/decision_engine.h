#ifndef DECISION_ENGINE_H
#define DECISION_ENGINE_H

#include "error.h"

typedef enum {
    DECISION_AUTO_FIX,
    DECISION_EDIT,
    DECISION_REJECT
} DecisionType;

/* classification */
int is_safe_error(ShrijiErrorCode code);
int is_ambiguous_error(ShrijiErrorCode code);

/* main decision */
DecisionType shriji_take_decision(
    int confidence,
    int risk,
    ShrijiErrorCode code
);

#endif
