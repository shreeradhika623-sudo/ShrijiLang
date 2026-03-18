#include "../include/ai_router.h"
#include "../include/ai_intent.h"

/* L3 */
#include "../pragya/contracts/l3_request.h"
#include "../pragya/contracts/l3_intent.h"
#include "../pragya/router/l3_router.h"

/*──────────────────────────────────────────────
  AI ROUTER — L3 (CLEAN)
  No Smriti. No Memory.
──────────────────────────────────────────────*/

L3Response ai_router_dispatch(const ShrijiBridgePacket *packet)
{
    L3Response fail = { NULL, 0 };

    if (!packet || !packet->text)
        return fail;

    AIIntent ai_intent = ai_detect_intent(packet->text);

    L3Intent l3_intent = L3_INTENT_UNKNOWN;

    switch (ai_intent)
    {
        case AI_INTENT_CALC:
            l3_intent = L3_INTENT_CALC;
            break;

        case AI_INTENT_EXPLAIN:
        case AI_INTENT_WHY:
            l3_intent = L3_INTENT_TEACH;
            break;

        case AI_INTENT_EMOTION:
            l3_intent = L3_INTENT_EMOTION;
            break;

        default:
            l3_intent = L3_INTENT_UNKNOWN;
            break;
    }

    L3Request req;
    req.raw_input = packet->text;
    req.intent    = l3_intent;

    return l3_route_request(&req);
}

/* backward compatibility */
void ai_router_process(const char *text)
{
    (void)text;
}
