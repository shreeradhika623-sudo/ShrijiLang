#include <string.h>
#include <stdio.h>

#include "sakhi.h"
#include "../tone/sakhi_tone.h"

/*──────────────────────────────────────────────
  INTERNAL STATIC BUFFER
  - single speak per call
  - no heap allocation
──────────────────────────────────────────────*/
static char SPEAK_BUFFER[512];

/*──────────────────────────────────────────────
  L3 SPEAK IMPLEMENTATION
──────────────────────────────────────────────*/
SakhiOutput sakhi_speak(const SakhiInput *in)
{
    SakhiOutput out;
    out.spoken = "";

    /* safety */
    if (!in || !in->text || !*in->text) {
        out.spoken = "Baat clear nahi hai. Thoda seedha likho.";
        return out;
    }

    /* apply tone filter (NO emotion injection) */
    const char *filtered = sakhi_tone_filter(in->text);

    /* copy into static buffer */
    strncpy(SPEAK_BUFFER, filtered, sizeof(SPEAK_BUFFER) - 1);
    SPEAK_BUFFER[sizeof(SPEAK_BUFFER) - 1] = '\0';

    out.spoken = SPEAK_BUFFER;
    return out;
}
