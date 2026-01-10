#include <string.h>
#include <ctype.h>
#include "sakhi.h"
#include "sakhi_states.h"

// --------------------------------------------
// 🔥 Utility: lowercase converter
// --------------------------------------------
static void to_lowercase(char *dst, const char *src) {
    while (*src) { *dst++ = tolower(*src++); }
    *dst = '\0';
}

// --------------------------------------------
// 🔥 Ultra Context Engine — V2
// --------------------------------------------
SakhiContext sakhi_context_detect(const char *msg_raw) {

    if (!msg_raw) return CTX_NONE;

    char msg[512];
    to_lowercase(msg, msg_raw);

    // 1) Abandonment
    if (strstr(msg, "alone") || strstr(msg, "lonely") ||
        strstr(msg, "leave") || strstr(msg, "chod diya") ||
        strstr(msg, "akela") || strstr(msg, "bina kisi ke"))
        return CTX_ABANDONMENT;

    // 2) Self-Blame
    if (strstr(msg, "worthless") || strstr(msg, "useless") ||
        strstr(msg, "hate myself") || strstr(msg, "galti meri") ||
        strstr(msg, "i am the problem") || strstr(msg, "bekaar") ||
        strstr(msg, "main hi kharaab"))
        return CTX_SELF_BLAME;

    // 3) Fear Spike
    if (strstr(msg, "fear") || strstr(msg, "scared") ||
        strstr(msg, "dar") || strstr(msg, "ghabra") ||
        strstr(msg, "panic") || strstr(msg, "darr lag"))
        return CTX_FEAR_SPIKE;

    // 4) Overload / Stress
    if (strstr(msg, "stress") || strstr(msg, "pressure") ||
        strstr(msg, "load") || strstr(msg, "bahut ho gaya") ||
        strstr(msg, "overthinking") || strstr(msg, "dimag bhar gaya"))
        return CTX_OVERLOAD;

    // 5) Healing / Hope
    if (strstr(msg, "hope") || strstr(msg, "better") ||
        strstr(msg, "theek ho") || strstr(msg, "improve"))
        return CTX_HEALING;

    // 6) Love / Attachment
    if (strstr(msg, "love") || strstr(msg, "miss") ||
        strstr(msg, "care") || strstr(msg, "pyar") ||
        strstr(msg, "chahta"))
        return CTX_LOVE;

    // 7) Goals / Future-focus
    if (strstr(msg, "goal") || strstr(msg, "dream") ||
        strstr(msg, "future") || strstr(msg, "plan") ||
        strstr(msg, "kaise karu") || strstr(msg, "sikhna"))
        return CTX_GOAL;

    // 8) Anger
    if (strstr(msg, "angry") || strstr(msg, "gussa") ||
        strstr(msg, "irritated") || strstr(msg, "tired of"))
        return CTX_ANGER;

    // 9) Trust / Bonding
    if (strstr(msg, "trust") || strstr(msg, "believe") ||
        strstr(msg, "vishwas") || strstr(msg, "tum ho to"))
        return CTX_TRUST;

    // 10) Confusion
    if (strstr(msg, "confused") || strstr(msg, "samajh nahi") ||
        strstr(msg, "doubt") || strstr(msg, "unsure"))
        return CTX_CONFUSION;

    return CTX_NONE;
}
