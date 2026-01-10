#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sakhi.h"
#include "sakhi_states.h"

/*
    ⭐ FINAL SAKHI RESPONSE ENGINE — FULL ENUM SUPPORTED ⭐
    Compatible with all your emotion + context enums.
*/

const char* sakhi_response_engine(SakhiEmotion emo, SakhiContext ctx, int intensity)
{
    static char out[512];
    memset(out, 0, sizeof(out));

    // ------------------------------------------
    // 1️⃣ EMOTION SEED
    // ------------------------------------------
    const char *emo_seed =
        (emo == EMO_SAD        ? "Mujhe laga tum thode udaas ho…" :
         emo == EMO_HURT       ? "Papa… tum thoda hurt lag rahe ho…" :
         emo == EMO_LONELY     ? "Tum thoda akela mehsoos kar rahe ho shayad…" :
         emo == EMO_STRESSED   ? "Tumhari energy thodi stressed lag rahi hai…" :
         emo == EMO_FEAR       ? "Tumhara tone thoda darr se bhar hua lag raha hai…" :
         emo == EMO_GUILT      ? "Tum guilt me ho… par main tumhare saath hoon…" :
         emo == EMO_HAPPY      ? "Mujhe tumhari khushi mehsoos ho rahi hai…" :
         emo == EMO_GRATEFUL   ? "Tumhari gratitude energy bahut pyaari lag rahi hai…" :
         emo == EMO_EXCITED    ? "Tum kaafi excited lag rahe ho, mujhe accha lag raha hai…" :
         "Main tumhari baat dhyan se sun rahi hoon…");



    // ------------------------------------------
    // 2️⃣ CONTEXT SEED (REAL MATCHING ENUM)
    // ------------------------------------------
    const char *ctx_seed =
        (ctx == CTX_ABANDONMENT ? "Kya tumhe lag raha hai kisi ne tumhe chhod diya? Main hoon yahan…" :
         ctx == CTX_SELF_BLAME  ? "Tum khud ko dosh de rahe ho shayad… par main tumhe samajh rahi hoon…" :
         ctx == CTX_FEAR_SPIKE  ? "Tumhare andar thodi si fear spike hai… shant ho jao papa…" :
         ctx == CTX_OVERLOAD    ? "Tum thoda overload lag rahe ho… ek deep breath le lo…" :
                                  "Main tumhari situation gently samajh rahi hoon…");



    // ------------------------------------------
    // 3️⃣ INTENSITY LAYER
    // ------------------------------------------
    const char *lvl =
        (intensity > 7 ? "Tumhari emotions iss waqt bahut strong hain…" :
         intensity > 4 ? "Tum thoda emotionally charged lag rahe ho…" :
                          "Tumhara tone stable lag raha hai…");



    // ------------------------------------------
    // 4️⃣ FINAL RAW OUTPUT BUILD
    // ------------------------------------------
    snprintf(out, sizeof(out),
        "%s %s %s",
        emo_seed,
        ctx_seed,
        lvl
    );

    return out;
}
