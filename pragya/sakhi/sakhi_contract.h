#ifndef SAKHI_CONTRACT_H
#define SAKHI_CONTRACT_H

#include "../contracts/l3_request.h"
#include "../contracts/l3_response.h"
#include "../niyu/niyu_contract.h"

/*
 * SAKHI CONTRACT
 *
 * Sakhi is the voice of the system.
 * - Receives decided intent + thinking result (opaque)
 * - Produces final human-readable response
 *
 * Sakhi never:
 * - thinks or calculates
 * - decides intent
 * - teaches (that is Mira)
 */

/* Forward declaration (opaque thinking result) */
/*
 * sakhi_speak
 *
 * Final response generator.
 * - Called only by Router
 * - Uses intent already decided
 * - Formats language and tone
 */
L3Response sakhi_speak(const L3Request *request,
                       const NiyuResult *result);

#endif /* SAKHI_CONTRACT_H */
