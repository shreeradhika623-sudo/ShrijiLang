#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sakhi_tone.h"

// Helper → wrap Sakhi style
static char* apply_sakhi_style(const char* text) {

    const char* prefix = "Papa… ";
    const char* suffix = " hai naa? ❤️";

    char* output = malloc(strlen(prefix) + strlen(text) + strlen(suffix) + 1);
    sprintf(output, "%s%s%s", prefix, text, suffix);

    return output;
}

// Main tone filter
const char* sakhi_tone_filter(const char* raw) {

    if (!raw || strlen(raw) == 0)
        return "Papa… main samajh nahi paayi… hai naa? ❤️";

    // Common soft corrections
    if (strstr(raw, "error"))
        return apply_sakhi_style("thoda sa confusion lag raha hai… chalo main softly samjhaun…");

    if (strstr(raw, "incorrect"))
        return apply_sakhi_style("shayad thoda galat direction me gaya tha… koi baat nahi…");

    if (strstr(raw, "because"))
        return apply_sakhi_style("iska reason main softly clear karti hoon…");

    // Default
    return apply_sakhi_style(raw);
}
