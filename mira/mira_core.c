#include <string.h>
#include <stdio.h>

const char* mira_process(const char *msg) {
    static char out[256];
    memset(out, 0, sizeof(out));

    if (!msg) {
        strcpy(out, "[Mira📐] Error: No input.");
        return out;
    }

    int a, b;
    if (sscanf(msg, "%d+%d", &a, &b) == 2) {
        snprintf(out, sizeof(out),
                 "[Mira📐] %d + %d = %d",
                 a, b, a+b);
        return out;
    }

    if (strstr(msg, "solve")) {
        strcpy(out, "[Mira📐] Solve what? Give a math expression.");
        return out;
    }

    snprintf(out, sizeof(out),
             "[Mira📐] I see no math or learning pattern.");
    return out;
}
