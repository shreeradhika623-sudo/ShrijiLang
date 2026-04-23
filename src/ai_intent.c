#include <string.h>
#include <ctype.h>

#include "../include/ai_intent.h"

/* lowercase helper */
static void to_lower(char *s)
{
    for (; *s; s++)
        *s = (char)tolower(*s);
}

/* simple substring match (L3 — lightweight, safe) */
static int has_word(const char *msg, const char *w)
{
    return strstr(msg, w) != NULL;
}

AIIntent ai_detect_intent(const char *message)
{
    if (!message || !*message)
        return AI_INTENT_UNKNOWN;

    char buf[256];
    strncpy(buf, message, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    to_lower(buf);

    /*──────────────────────────────────────────────
      EXPLAIN / SAMJHAO (LOGIC / TEACHING)
      → Niyu (primary explain engine)
    ──────────────────────────────────────────────*/
    if (has_word(buf, "samjhao") ||
        has_word(buf, "samjha")  ||
        has_word(buf, "samajhao")||
        has_word(buf, "samajh")  ||
        has_word(buf, "explain") ||
        has_word(buf, "what is") ||
        has_word(buf, "kya hota"))
        return AI_INTENT_EXPLAIN;

    /*──────────────────────────────────────────────
      WHY / REASON
    ──────────────────────────────────────────────*/
    if (has_word(buf, "why")  ||
        has_word(buf, "kyun") ||
        has_word(buf, "kyu")  ||
        has_word(buf, "kyon"))
        return AI_INTENT_WHY;

    /*──────────────────────────────────────────────
      CALC / MATH
    ──────────────────────────────────────────────*/
    if (has_word(buf, "calculate") ||
        has_word(buf, "solve")     ||
        has_word(buf, "math")      ||
        has_word(buf, "hisab"))
        return AI_INTENT_CALC;

    /*──────────────────────────────────────────────
      EMOTION / FEELING
    ──────────────────────────────────────────────*/
    if (has_word(buf, "sad")    ||
        has_word(buf, "dukhi")  ||
        has_word(buf, "lonely") ||
        has_word(buf, "akela")  ||
        has_word(buf, "dard"))
        return AI_INTENT_EMOTION;

    return AI_INTENT_UNKNOWN;
}
