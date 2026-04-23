#include "../include/decision_engine.h"

/* SAFE FIXABLE ERRORS */
int is_safe_error(ShrijiErrorCode code)
{
    switch(code)
    {
        case E_PARSE_MISSING_OPERATOR:
        case E_PARSE_DOUBLE_OPERATOR:
            return 1;
        default:
            return 0;
    }
}

/* AMBIGUOUS ERRORS */
int is_ambiguous_error(ShrijiErrorCode code)
{
    switch(code)
    {
        case E_PARSE_INVALID_TOKEN:
        case E_PARSE_02:
            return 1;
        default:
            return 0;
    }
}

/* MAIN DECISION ENGINE */
DecisionType shriji_take_decision(
    int confidence,
    int risk,
    ShrijiErrorCode code
)
{
    (void)confidence;
    (void)risk;

    /* SAFE → always auto fix */
    if (is_safe_error(code))
        return DECISION_AUTO_FIX;

    /* AMBIGUOUS → ask user */
    if (is_ambiguous_error(code))
        return DECISION_EDIT;

    /* REST → reject */
    return DECISION_REJECT;
}
