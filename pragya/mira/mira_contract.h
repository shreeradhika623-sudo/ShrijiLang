#ifndef MIRA_CONTRACT_H
#define MIRA_CONTRACT_H

#include "../contracts/l3_request.h"
#include "../contracts/l3_response.h"
#include "../niyu/niyu_contract.h"
#include "../../include/pragya_avastha.h"

/*
 * MIRA CONTRACT
 *
 * Mira is the teaching layer.
 * - Activated only when teaching_mode == 1
 * - Provides structured explanation / lesson
 *
 * Mira never:
 * - handles normal conversation
 * - decides intent
 * - performs deep logic (that is Niyu)
 */

/*
 * Legacy interface (old L3 system)
 */
L3Response mira_teach(const L3Request *request);

/*
 * New ShrijiLang teaching interface
 * Used by Pragya Router
 */

void mira_teach_avastha(
    PragyaAvastha *avastha,
    NiyuResult *result
);

#endif /* MIRA_CONTRACT_H */
