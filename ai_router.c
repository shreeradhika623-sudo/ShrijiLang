#include <string.h>
#include <stdio.h>

#include "include/ai_router.h"

#include "sakhi/sakhi_bridge.h"
#include "niyu/niyu_bridge.h"
#include "mira/mira.h"
#include "kavya/kavya.h"
#include "tone/sakhi_tone.h"

/*──────────────────────────────────────────────────────────────
  SHRIJILANG AI ROUTER — HUMAN INTERACTION LAYER (L3 LOCKED)

  FINAL LAWS (DO NOT VIOLATE):
  1. ai_router NEVER talks to Shiri
  2. ai_router NEVER executes language logic
  3. ai_router ONLY handles chat / conversation
  4. Interpreter is the ONLY executor

  This file is PURE PRESENTATION + ROUTING.
──────────────────────────────────────────────────────────────*/


/*──────────────────────────────────────────────
  GLOBAL ROUTER MODE (L3: WHO SPEAKS — HINT ONLY)
──────────────────────────────────────────────*/
static AIRouterMode CURRENT_ROUTER_MODE = AI_ROUTER_MODE_AUTO;


/*──────────────────────────────────────────────
  INTERNAL HELPERS
──────────────────────────────────────────────*/

// detect math / computation intent → Mira
static int is_math_query(const char *msg)
{
    int a, b;
    if (sscanf(msg, "%d+%d", &a, &b) == 2) return 1;
    if (strstr(msg, "solve")) return 1;
    if (strstr(msg, "math")) return 1;
    if (strstr(msg, "compute")) return 1;
    return 0;
}

// trim leading spaces
static const char *trim(const char *s)
{
    while (*s == ' ') s++;
    return s;
}


/*──────────────────────────────────────────────
  PRESENTATION LAYER (NO SHIRI HERE)
──────────────────────────────────────────────*/

static void present_as_sakhi(const char *msg)
{
    const char *out = sakhi_bridge(msg);
    if (out)
        printf("%s\n", sakhi_tone_filter(out));
}

static void present_as_niyu(const char *msg)
{
    niyu_bridge(msg);
}

static void present_as_mira(const char *msg)
{
    const char *out = mira_process(msg);
    if (out)
        printf("%s\n", out);
}

static void present_as_kavya(const char *msg)
{
    kavya_process(msg);
}


/*──────────────────────────────────────────────
  MAIN ROUTER — CHAT ONLY
──────────────────────────────────────────────*/
void ai_router_process(const char *message)
{
    if (!message || !*message)
        return;

    /*──────────────────────────────────────
      EXPLICIT PREFIX ROUTING
    ──────────────────────────────────────*/
    if (strncmp(message, "sakhi", 5) == 0) {
        present_as_sakhi(trim(message + 5));
        return;
    }

    if (strncmp(message, "niyu", 4) == 0) {
        present_as_niyu(trim(message + 4));
        return;
    }

    if (strncmp(message, "mira", 4) == 0) {
        present_as_mira(trim(message + 4));
        return;
    }

    if (strncmp(message, "kavya", 5) == 0) {
        present_as_kavya(trim(message + 5));
        return;
    }

    /*──────────────────────────────────────
      AUTO ROUTING (DEFAULT BEHAVIOR)
    ──────────────────────────────────────*/

    // math / computation → Mira
    if (is_math_query(message)) {
        present_as_mira(message);
        return;
    }

    // logic / reasoning → Niyu
    if (strstr(message, "why") ||
        strstr(message, "reason") ||
        strstr(message, "logic")) {
        present_as_niyu(message);
        return;
    }

    // emotion / pain → Sakhi
    if (strstr(message, "sad") ||
        strstr(message, "hurt") ||
        strstr(message, "lonely")) {
        present_as_sakhi(message);
        return;
    }

    // default → Sakhi
    present_as_sakhi(message);
}


/*──────────────────────────────────────────────
  ROUTER MODE CONTROL (HOOK ONLY)
──────────────────────────────────────────────*/

void ai_router_set_mode(AIRouterMode mode)
{
    CURRENT_ROUTER_MODE = mode;
}

AIRouterMode ai_router_get_mode(void)
{
    return CURRENT_ROUTER_MODE;
}
