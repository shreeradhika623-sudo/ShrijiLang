#include <stdio.h>
#include <string.h>

#include "../include/ai_router.h"
#include "../include/ai_intent.h"

#include "../pragya/contracts/l3_request.h"
#include "../pragya/contracts/l3_response.h"
#include "../pragya/contracts/l3_intent.h"

/*
   Legacy AI Router

   NOTE:
   ShrijiLang now uses KRST + Pragya Router
   for all intelligence routing.

   This module is preserved only for
   future conversational AI layer.
*/

L3Response ai_router_dispatch(const ShrijiBridgePacket *pkt)
{
    L3Response resp;

    resp.text = NULL;
    resp.success = 0;

    if (!pkt || !pkt->text)
        return resp;

    /* For now just acknowledge input */

    resp.text = "AI router currently inactive.";
    resp.success = 1;

    return resp;
}
