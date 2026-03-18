#ifndef L3_REQUEST_H
#define L3_REQUEST_H

#include "l3_intent.h"

/*
 * L3Request:
 * Immutable input structure.
 * Router decides intent.
 * No teaching_mode flag here.
 * Behavior intent se hi control hoga.
 */

typedef struct {
    const char *raw_input;     /* original user text */
    L3Intent    intent;        /* decided by router */
} L3Request;

#endif /* L3_REQUEST_H */
