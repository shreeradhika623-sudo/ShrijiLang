#include "l3_router.h"
#include "../niyu/niyu_contract.h"
#include "../sakhi/sakhi_contract.h"
#include "../mira/mira_contract.h"

/*
 * L3 Router Implementation
 *
 * This is minimal, lawful wiring.
 * No deep logic is allowed here.
 */

L3Intent l3_decide_intent(const char *user_input)
{
    (void)user_input;
    /* Placeholder: default intent */
    return L3_INTENT_UNKNOWN;
}

int l3_is_teaching_allowed(const char *user_input)
{
    (void)user_input;
    /* Placeholder: teaching disabled by default */
    return 0;
}

L3Response l3_route_request(const char *user_input)
{
    L3Request request;
    request.raw_input     = user_input;
    request.intent        = l3_decide_intent(user_input);
    request.teaching_mode = l3_is_teaching_allowed(user_input);

    /* Teaching flow */
    if (request.teaching_mode) {
        return mira_teach(&request);
    }

    /* Thinking flow */
    NiyuResult *result = niyu_think(&request);

    /* Speaking flow */
    L3Response response = sakhi_speak(&request, result);

    /* Cleanup */
    niyu_free_result(result);

    return response;
}
