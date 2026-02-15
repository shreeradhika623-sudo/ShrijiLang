#include <ctype.h>
#include <string.h>

/* check: string is only number */
static int is_pure_number(const char *s)
{
    if (!s || !*s) return 0;

    if (*s == '-' || *s == '+') s++;

    while (*s) {
        if (!isdigit(*s)) return 0;
        s++;
    }
    return 1;
}

const char* sakhi_tone_filter(const char* raw)
{
    if (!raw || !*raw)
        return "Baat clear nahi hai.";

    /* 🔒 L3 RULE:
       factual output (numbers) → no tone */
    if (is_pure_number(raw)) {
        return raw;
    }

    /* normal neutral speak */
    return raw;
}
