#ifndef L3_ROUTER_H
#define L3_ROUTER_H

#include "../contracts/l3_request.h"
#include "../contracts/l3_response.h"

/*
 * L3 Router
 *
 * Central authority of L3.
 * - Decides intent
 * - Chooses thinker (Niyu)
 * - Chooses speaker (Sakhi / Mira)
 *
 * Router never thinks deeply.
 * Router never formats final language.
 * Router only coordinates.
 */

/*
 * l3_route_request
 *
 * Entry point for all L3 requests.
 * - Takes raw user input
 * - Builds L3Request
 * - Dispatches to correct flow
 * - Returns final L3Response
 */
L3Response l3_route_request(const char *user_input);

/*
 * l3_decide_intent
 *
 * Router-only function.
 * - Examines user input
 * - Sets L3Intent
 */
L3Intent l3_decide_intent(const char *user_input);

/*
 * l3_is_teaching_allowed
 *
 * Checks whether Mira is allowed.
 * - Explicit user request only
 */
int l3_is_teaching_allowed(const char *user_input);

#endif /* L3_ROUTER_H */
