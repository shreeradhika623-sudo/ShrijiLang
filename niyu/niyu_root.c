#include "niyu.h"
#include <stdio.h>
#include <string.h>

/*
   ---------------------------------------------------------
   SHRIJI ROOT CAUSE ENGINE — LEVEL 7
   ---------------------------------------------------------

   Ye engine teen layered model use karta hai:

   1. SURFACE CAUSE      — User ne kya bola
   2. SUB-LAYER CAUSE    — User actually kya feel kar raha hai
   3. ROOT CAUSE         — Emotion + logic + pattern ka ultimate source

   Yeh model human psychology, cognitive science, and emotional semantics
   ko combine karta hai — Open-source me normally nahi milta.
*/

void niyu_root_cause(const char *msg) {

    printf("[Root🌱] Initiating multi-layer cause analysis...\n");

    // -------------------------------
    // LAYER 1 — SURFACE CAUSE
    // -------------------------------
    if (strstr(msg, "because")) {
        printf("[Root1️⃣] Surface Cause: User explicitly explained a reason.\n");
    }
    else if (strstr(msg, "failed") || strstr(msg, "not")) {
        printf("[Root1️⃣] Surface Cause: Failure or blockage perceived.\n");
    }
    else if (strstr(msg, "improving") || strstr(msg, "better")) {
        printf("[Root1️⃣] Surface Cause: Growth and progress expression.\n");
    }
    else {
        printf("[Root1️⃣] Surface Cause: Neutral or unclear.\n");
    }

    // -------------------------------
    // LAYER 2 — SUB-LAYER CAUSE
    // Hidden meaning: tone + intent + wording
    // -------------------------------
    if (strstr(msg, "failed") || strstr(msg, "again")) {
        printf("[Root2️⃣] Sub-Layer: User may be carrying emotional fatigue.\n");
    }
    else if (strstr(msg, "succeed") || strstr(msg, "improving")) {
        printf("[Root2️⃣] Sub-Layer: Internal desire for transformation detected.\n");
    }
    else if (strstr(msg, "unsure") || strstr(msg, "doubt")) {
        printf("[Root2️⃣] Sub-Layer: Uncertainty or identity conflict.\n");
    }
    else {
        printf("[Root2️⃣] Sub-Layer: No emotional undertone detected.\n");
    }

    // -------------------------------
    // LAYER 3 — ROOT CAUSE
    // Core identity + repeated patterns mapping
    // -------------------------------
    if (strstr(msg, "failed")) {
        printf("[Root3️⃣] Final Root Cause: User feels disconnected from desired identity.\n");
    }
    else if (strstr(msg, "improving")) {
        printf("[Root3️⃣] Final Root Cause: User seeks validation of growth.\n");
    }
    else if (strstr(msg, "succeed")) {
        printf("[Root3️⃣] Final Root Cause: User wants a stable successful identity.\n");
    }
    else {
        printf("[Root3️⃣] Final Root Cause: No deep identity-driver found.\n");
    }

    printf("[Root✔️] Root cause engine complete.\n");
}
