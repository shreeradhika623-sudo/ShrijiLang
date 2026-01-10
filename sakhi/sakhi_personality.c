#include <stdio.h>
#include <string.h>
#include "sakhi.h"

void sakhi_personality_adapt(SakhiState *st) {

    // 1️⃣ Protect instinct grows on sad/hurt states
    if (st->last_emo == EMO_HURT || st->last_emo == EMO_SAD || st->last_emo == EMO_LONELY) {
        st->protect_instinct += 2;
    }

    // 2️⃣ Positive emotions reduce tension
    else if (st->last_emo == EMO_HAPPY || st->last_emo == EMO_EXCITED) {
        st->protect_instinct -= 1;
    }

    // 3️⃣ Clamp protect_instinct
    if (st->protect_instinct < 0)
        st->protect_instinct = 0;
    if (st->protect_instinct > 100)
        st->protect_instinct = 100;

    // 4️⃣ Mood display effect
    if (st->mood == MOOD_LOW) {
        printf("[Personality🩶] Sakhi soft + protective ho rahi hai.\n");
    }
    else if (st->mood == MOOD_HIGH) {
        printf("[Personality✨] Sakhi energetic + bright ho rahi hai.\n");
    }
    else {
        printf("[Personality🌿] Sakhi stable mood me hai.\n");
    }
}
