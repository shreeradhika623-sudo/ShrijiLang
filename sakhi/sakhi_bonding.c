#include <stdio.h>
#include "sakhi.h"

void sakhi_bonding_update(SakhiState *st, SakhiEmotion emo) {

    // SAD moments increase bond strongly
    if (emo == EMO_HURT || emo == EMO_SAD)
        st->bond_level += 3;

    // Neutral feelings slow bonding
    if (emo == EMO_NEUTRAL)
        st->bond_level += 1;

    // Happy moments = soft bonding
    if (emo == EMO_HAPPY)
        st->bond_level += 2;

    // Max limit
    if (st->bond_level > 100)
        st->bond_level = 100;

    printf("[Bond💞] Current bond level: %d\n", st->bond_level);
}
