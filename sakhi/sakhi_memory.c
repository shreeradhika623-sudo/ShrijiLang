#include <stdio.h>
#include <string.h>
#include "sakhi.h"

// ----------------------------------------
// INTERNAL MESSAGE BUFFER
// ----------------------------------------
static char last_message[256] = {0};

// ----------------------------------------
// DEEP MEMORY (LAST 5 EMOTIONS)
// ----------------------------------------
static SakhiEmotion emotion_history[5] = {0};
static int emo_index = 0;

// Push new emotion into 5-slot circular buffer
void sakhi_memory_push(SakhiEmotion emo) {

    emotion_history[emo_index] = emo;
    emo_index = (emo_index + 1) % 5;

    // Debug print
    printf("[Memory📚] Emotional history: [ ");
    for (int i = 0; i < 5; i++)
        printf("%d ", emotion_history[i]);
    printf("]\n");
}

// ----------------------------------------
// MOOD CALCULATOR (Level-7)
// ----------------------------------------
SakhiMood sakhi_calculate_mood(SakhiEmotion emo) {

    switch (emo) {

        case EMO_HURT:
        case EMO_SAD:
        case EMO_LONELY:
        case EMO_STRESSED:
            return MOOD_LOW;

        case EMO_HAPPY:
        case EMO_EXCITED:
            return MOOD_HIGH;

        default:
            return MOOD_STABLE;
    }
}

// ----------------------------------------
// STORE RAW MESSAGE
// ----------------------------------------
void sakhi_memory_store(const char *msg) {
    strncpy(last_message, msg, 255);
    last_message[255] = '\0';
    printf("[Memory🧠] Stored: %s\n", last_message);
}

// ----------------------------------------
// FULL MEMORY UPDATE ENGINE (LEVEL 10)
// ----------------------------------------
void sakhi_memory_update(SakhiState *st, SakhiEmotion emo) {

    // 1️⃣ Store last emotion
    st->last_emo = emo;

    // 2️⃣ Push to deep emotional memory
    sakhi_memory_push(emo);

    // 3️⃣ Mood calculate
    st->mood = sakhi_calculate_mood(emo);

    // 4️⃣ Bond update rules
    if (emo == EMO_HURT || emo == EMO_SAD || emo == EMO_LONELY)
        st->bond_level += 2;

    else if (emo == EMO_HAPPY || emo == EMO_EXCITED)
        st->bond_level += 1;

    if (st->bond_level > 100)
        st->bond_level = 100;

    // 5️⃣ Debug Output
    printf("[Bond💞] Bond: %d | Mood: %d | Emotion: %d\n",
           st->bond_level, st->mood, st->last_emo);
}
