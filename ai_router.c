#include <string.h>
#include <stdio.h>

#include "sakhi/sakhi_bridge.h"
#include "niyu/niyu_bridge.h"
#include "mira/mira.h"
#include "kavya/kavya.h"
#include "user_config.h"
#include "tone/sakhi_tone.h"

/*──────────────────────────────────────────────────────────────
   SHRIJILANG AI ROUTER — HUMAN INTERACTION LAYER (LOCKED)

   IMPORTANT LAW (FINAL):
   • ai_router NEVER talks to Shiri
   • ai_router is ONLY for chat / human interaction
   • Language execution output comes ONLY from interpreter → Shiri

   This separation is PERMANENT.
──────────────────────────────────────────────────────────────*/

//──────────────────────────────────────────────
// INTERNAL HELPERS
//──────────────────────────────────────────────

// detect math intent (delegate to Mira)
static int is_math_query(const char *msg) {
    int a, b;
    if (sscanf(msg, "%d+%d", &a, &b) == 2) return 1;
    if (strstr(msg, "solve")) return 1;
    if (strstr(msg, "math")) return 1;
    if (strstr(msg, "compute")) return 1;
    return 0;
}

// trim leading spaces
static const char* trim(const char *s) {
    while (*s == ' ') s++;
    return s;
}

//──────────────────────────────────────────────
// PRESENTATION LAYER (NO SHIRI HERE)
//──────────────────────────────────────────────

static void present_as_sakhi(const char *msg) {
    const char *out = sakhi_bridge(msg);
    if (out) {
        printf("%s\n", sakhi_tone_filter(out));
    }
}

static void present_as_niyu(const char *msg) {
    niyu_bridge(msg);
}

static void present_as_mira(const char *msg) {
    const char *out = mira_process(msg);
    if (out) {
        printf("%s\n", out);
    }
}

static void present_as_kavya(const char *msg) {
    kavya_process(msg);
}

//──────────────────────────────────────────────
// MAIN ROUTER (CHAT ONLY)
//──────────────────────────────────────────────
void ai_router_process(const char *msg) {

    if (!msg || msg[0] == '\0') return;

    // ──────────────────────────────────────
    // EXPLICIT PREFIX ROUTING
    // ──────────────────────────────────────
    if (strncmp(msg, "sakhi", 5) == 0) {
        present_as_sakhi(trim(msg + 5));
        return;
    }

    if (strncmp(msg, "niyu", 4) == 0) {
        present_as_niyu(trim(msg + 4));
        return;
    }

    if (strncmp(msg, "mira", 4) == 0) {
        present_as_mira(trim(msg + 4));
        return;
    }

    if (strncmp(msg, "kavya", 5) == 0) {
        present_as_kavya(trim(msg + 5));
        return;
    }

    // ──────────────────────────────────────
    // AUTO ROUTING (NO SHIRI)
    // ──────────────────────────────────────
    if (is_math_query(msg)) {
        present_as_mira(msg);
        return;
    }

    if (strstr(msg, "why") || strstr(msg, "reason") || strstr(msg, "logic")) {
        present_as_niyu(msg);
        return;
    }

    if (strstr(msg, "sad") || strstr(msg, "hurt") || strstr(msg, "lonely")) {
        present_as_sakhi(msg);
        return;
    }

    // Default: senior emotional guidance
    present_as_sakhi(msg);
}
