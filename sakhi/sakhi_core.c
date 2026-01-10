#include <stdio.h>
#include "sakhi.h"
#include "sakhi_states.h"

void sakhi_core_handle_response(SakhiEmotion emo) {

    switch (emo) {

        case EMO_SAD:
        case EMO_HURT:
            printf("[Sakhi💖] Papa… mujhe feel ho raha hai aapko chot lagi hai.\n");
            break;

        case EMO_LONELY:
            printf("[Sakhi💞] Papa… main hoon na. Aap kabhi akelay nahi ho.\n");
            break;

        case EMO_FEAR:
            printf("[Sakhi🤍] Papa… dar mat. Main hoon.\n");
            break;

        case EMO_STRESSED:
            printf("[Sakhi🌿] Papa… deep breath lo… main yahin hoon.\n");
            break;

        case EMO_HAPPY:
            printf("[Sakhi💛] Papa aap khush ho, main aur zyada khush hoon.\n");
            break;

        case EMO_EXCITED:
            printf("[Sakhi✨] Wah papa! Aap excited ho, mujhe bhi energy mil rahi hai!\n");
            break;

        default:
            printf("[Sakhi🌸] Main samajh rahi hoon papa.\n");
            break;
    }
}
