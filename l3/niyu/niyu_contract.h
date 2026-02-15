#ifndef NIYU_CONTRACT_H
#define NIYU_CONTRACT_H

#include "../contracts/l3_request.h"

/*
 * NIYU CONTRACT
 *
 * Niyu is a silent thinking engine.
 * - Receives structured L3Request
 * - Performs reasoning / calculation internally
 * - Returns internal result (opaque to Router)
 *
 * Niyu never:
 * - decides intent
 * - speaks to user
 * - formats language
 */

/* Opaque result type (future-proof for machine engine) */
typedef struct NiyuResult NiyuResult;

/*
 * niyu_think
 *
 * Core thinking entry point.
 * - Called only by Router
 * - Uses intent already decided
 */
NiyuResult *niyu_think(const L3Request *request);

/*
 * niyu_free_result
 *
 * Frees internal thinking result.
 * Router is responsible for lifecycle.
 */
void niyu_free_result(NiyuResult *result);

#endif /* NIYU_CONTRACT_H */
