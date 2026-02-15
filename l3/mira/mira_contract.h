#ifndef MIRA_CONTRACT_H
#define MIRA_CONTRACT_H

#include "../contracts/l3_request.h"
#include "../contracts/l3_response.h"

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
 * mira_teach
 *
 * Teaching entry point.
 * - Called only by Router
 * - teaching_mode must be enabled
 */
L3Response mira_teach(const L3Request *request);

#endif /* MIRA_CONTRACT_H */
