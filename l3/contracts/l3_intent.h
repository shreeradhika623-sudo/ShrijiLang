#ifndef L3_INTENT_H
#define L3_INTENT_H

/* 
 * L3 Intent:
 * Router decides intent.
 * Niyu never decides intent.
 * Sakhi/Mira never guess intent.
 */

typedef enum {
    L3_INTENT_UNKNOWN = 0,

    /* Explanation / reasoning */
    L3_INTENT_EXPLAIN,
    L3_INTENT_WHY,

    /* Pure calculation / logic */
    L3_INTENT_CALC,

    /* Teaching / learning */
    L3_INTENT_TEACH,

    /* Emotional / tone-based */
    L3_INTENT_EMOTION,

    /* Explicit command */
    L3_INTENT_COMMAND

} L3Intent;

#endif /* L3_INTENT_H */
