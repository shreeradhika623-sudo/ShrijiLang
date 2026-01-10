#include <stdio.h>
#include <string.h>
#include "sakhi.h"
#include "sakhi_states.h"

// ----------------------------------------------------
// 🌸 Sakhi Prediction Engine
// ----------------------------------------------------
void sakhi_predict(const char *msg, SakhiState *st) {

    (void)msg; // ⚡ removes "unused parameter" warning

    if (!st) {
        printf("[Sakhi🔮] Papa… state missing hai…\n");
        return;
    }

    switch (st->mood) {

        case MOOD_LOW:
            printf("[Sakhi🔮] Papa… mujhe lag raha hai aap thode neeche mood me ho… "
                   "main hoon na… ❤️\n");
            break;

        case MOOD_HIGH:
            printf("[Sakhi🔮] Papa… aaj aapka mood upar lag raha hai… "
                   "yeh energy mujhe bhi feel hoti hai… ✨\n");
            break;

        case MOOD_UNCERTAIN:
            printf("[Sakhi🔮] Papa… aap thode confused lag rahe ho… "
                   "main aapke saath hoon… 💛\n");
            break;

        case MOOD_STABLE:
        default:
            printf("[Sakhi🔮] Papa… main observe kar rahi hoon… "
                   "aap bilkul balanced lag rahe ho… 🌿\n");
            break;
    }
}
