#include "../include/nirman.h"
#include <string.h>

/* ===============================
   GLOBAL CONTEXT
   =============================== */
NirmanContext NIRMAN_CTX = {0};

/* ===============================
   LIFECYCLE
   =============================== */

void nirman_start(void)
{
    NIRMAN_CTX.active = 1;

    /* reset flow */
    NIRMAN_CTX.flow_stage = 0;
    NIRMAN_CTX.goal[0] = '\0';
}

void nirman_stop(void)
{
    NIRMAN_CTX.active = 0;

    /* reset flow */
    NIRMAN_CTX.flow_stage = 0;
    NIRMAN_CTX.goal[0] = '\0';
}

int nirman_is_active(void)
{
    return NIRMAN_CTX.active;
}

/* ===============================
   INTERNAL HELPERS
   =============================== */

static void to_lower(char *s)
{
    for (int i = 0; s[i]; i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 32;
    }
}

/* ===============================
   INTENT ENGINE
   =============================== */

int nirman_detect_intent(const char *input)
{
    if (!input)
        return 0;

    char temp[256];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[255] = '\0';

    to_lower(temp);

    /* 1 = TALK */
    if (strstr(temp, "kaise") ||
        strstr(temp, "hello") ||
        strstr(temp, "hi") ||
        strstr(temp, "hy") ||
        strstr(temp, "radhe"))
    {
        return 1;
    }

    /* 2 = BUILD */
    if (strstr(temp, "bana") ||
        strstr(temp, "create") ||
        strstr(temp, "app") ||
        strstr(temp, "software"))
    {
        return 2;
    }

    /* 3 = ERROR / HELP */
    if (strstr(temp, "error") ||
        strstr(temp, "problem") ||
        strstr(temp, "issue"))
    {
        return 3;
    }

    return 0;
}
