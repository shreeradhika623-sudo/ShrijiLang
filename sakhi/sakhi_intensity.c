#include <string.h>
#include "sakhi.h"

// Basic intensity detection
int sakhi_intensity_level(const char *msg) {

    if (strstr(msg, "very") || strstr(msg, "bahut") || strstr(msg, "bohot"))
        return 2;

    if (strstr(msg, "extremely") || strstr(msg, "zyada"))
        return 3;

    return 1; // normal intensity
}
