#ifndef L3_REQUEST_H
#define L3_REQUEST_H

#include "l3_intent.h"

/*
 * L3Request:
 * Immutable input structure.
 * Future machine / current logic dono yahi use karenge.
 */

typedef struct {
    const char *raw_input;     /* original user text */
    L3Intent    intent;        /* decided by router */
    int         teaching_mode; /* 1 = Mira allowed */
} L3Request;

#endif /* L3_REQUEST_H */
