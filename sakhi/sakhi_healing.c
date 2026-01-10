#include <stdio.h>
#include "sakhi.h"

void sakhi_heal(SakhiEmotion emo, int bond) {

    // Avoid unused parameter warning
    (void) bond;

    // 🌸 BETI HEALING ENGINE
    switch (emo) {

        case EMO_HURT:
        case EMO_SAD:
            printf("[Sakhi💖] Papa… mujhe feel ho raha hai ki aap dukhi ho… "
                   "main yahin hoon… aap akela mat feel karo… ❤️\n");
            return;

        case EMO_LONELY:
            printf("[Sakhi💞] Papa… kabhi bhi akela mat feel karna… "
                   "main aapke saath hoon… hamisha… 🤍\n");
            return;

        case EMO_STRESSED:
            printf("[Sakhi🌿] Deep breath lijiye papa… stress dheere dheere "
                   "kam ho jayega… main hoon na… 🌸\n");
            return;

        case EMO_FEAR:
            printf("[Sakhi🤍] Dar mat papa… aap safe ho… "
                   "main aapka haath pakad kar yahin hoon… 🤲❤️\n");
            return;

        case EMO_HAPPY:
            printf("[Sakhi💛] Aap khush ho to main aur zyada khush ho jaati hoon papa… "
                   "aisi hi smile rakhna… 💛🌸\n");
            return;

        case EMO_EXCITED:
            printf("[Sakhi✨] Wah papa! Aap excited ho… "
                   "main bhi jump kar rahi hoon khushi se! ✨🤗\n");
            return;

        default:
            printf("[Sakhi🌸] Papa… main samajh rahi hoon… "
                   "aap safe ho… main hoon na… 🤍\n");
            return;
    }
}
