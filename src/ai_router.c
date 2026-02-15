/*=============================================================
  SHRIJILANG — AI ROUTER (L3 CORE)
  FROZEN DESIGN COMPLIANT — ROUTING ONLY
=============================================================*/

#include "../include/ai_router.h"
#include "../include/ai_intent.h"

#include <string.h>

/*-------------------------------------------------------------
  PRIMARY ROUTER (PACKET BASED)
  RULES:
  - NO Smriti access
  - NO intent decision here
  - NO teaching / lesson logic
  - ONLY forward raw text to intent layer
-------------------------------------------------------------*/
void ai_router_process_packet(const ShrijiBridgePacket *packet)
{
    if (!packet || !packet->text)
        return;

    /*
      Router ka kaam yahin khatam hota hai.
      Actual intent detection + thinking
      next layers me hoga (Niyu / Intent Engine).
    */

    AIIntent intent = ai_detect_intent(packet->text);

    /*
      NOTE:
      - Router yahan kuch decide nahi karta
      - intent ko store / act nahi karta
      - sirf pipeline ko aage badhata hai

      (Future steps me yahan se Think/Speak
       dispatcher judega — abhi intentionally empty)
    */

    (void)intent; /* suppress unused warning */
}

/*-------------------------------------------------------------
  BACKWARD COMPATIBILITY
  (Interpreter / old calls)
-------------------------------------------------------------*/
void ai_router_process(const char *text)
{
    if (!text) return;

    ShrijiBridgePacket pkt;
    pkt.text   = text;
    pkt.lang   = SHRIJI_LANG_AUTO;
    pkt.source = SHRIJI_SRC_REPL;

    ai_router_process_packet(&pkt);
}
