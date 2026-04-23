/* RADHE_RADHE_SHREEJI_RADHE_RANI */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/interpreter.h"
#include "../include/error.h"
#include "../include/fix_engine.h"
#include "../include/nirman.h"
#include "../include/parser.h"
#include "../include/value.h"
#include "../include/env.h"
#include "../include/runtime.h"
#include "../include/pragya_avastha.h"
#include "../include/decision_engine.h"

#include "krst_core.h"
#include "krst_types.h"

#include "../pragya/router/l3_router.h"
#include "../include/error_intelligence.h"


#include "../include/global.h"

#include "../include/user_config.h"

extern Env *GLOBAL_ENV;

/* ================= SESSION ================= */

static int session_confidence = 100;
static int session_risk = 4;
static int success_streak = 0;

/* ================= LABEL ================= */

static const char* teach_label(int level)
{
    switch(level)
    {
        case KRST_TEACH_SILENT:  return "SILENT";
        case KRST_TEACH_HINT:    return "HINT";
        case KRST_TEACH_EXPLAIN: return "EXPLAIN";
        case KRST_TEACH_DEEP:    return "DEEP";
        case KRST_TEACH_TRAIN:   return "TRAIN";
        default: return "SILENT";
    }
}

static const char* tone_label(int tone)
{
    switch(tone)
    {
        case KRST_TONE_NEUTRAL: return "NEUTRAL";
        case KRST_TONE_STRICT:  return "STRICT";
        case KRST_TONE_ALERT:   return "ALERT";
        default: return "NEUTRAL";
    }
}

/* ================= THRESHOLD ================= */

static void apply_thresholds(KRSTDecision *d)
{
    int c = d->confidence_score;

    if (c <= 20)      d->teaching_level = KRST_TEACH_TRAIN;
    else if (c <= 40) d->teaching_level = KRST_TEACH_DEEP;
    else if (c <= 60) d->teaching_level = KRST_TEACH_EXPLAIN;
    else if (c <= 80) d->teaching_level = KRST_TEACH_HINT;
    else              d->teaching_level = KRST_TEACH_SILENT;

    if (session_risk >= 76)
    {
        d->tone = KRST_TONE_ALERT;
        d->escalate = 1;
    }
    else if (session_risk >= 30)
    {
        d->tone = KRST_TONE_STRICT;
        d->escalate = 0;
    }
    else
    {
        d->tone = KRST_TONE_NEUTRAL;
        d->escalate = 0;
    }
}

/* ================= MAIN ================= */

int krst_process(PragyaAvastha *avastha)
{
    if (!avastha || !avastha->raw_text)
        return 1;

const char *input = avastha->raw_text;

/*  NIRMAN START */
if (strcmp(input, "nirman") == 0) {
    nirman_start();
    printf("Nirman mode activated\n");
    printf("Welcome to Shriji World\n");
    return 0;
}

/*  NIRMAN STOP */
if (strcmp(input, "exit nirman") == 0) {
    nirman_stop();
    printf("Nirman mode deactivated\n");
    return 0;
}

/*  NIRMAN MODE ROUTING */
if (nirman_is_active()) {

    int intent = nirman_detect_intent(input);

    printf("SHRIJI (intent=%d)\n", intent);

    return 0;
}


    if (DEV_MODE)
        printf("[KRST] Input: %s\n", input);

    error_reported = 0;
    avastha->stop_execution = 0;

    avastha->confidence = session_confidence;
    avastha->risk = session_risk;

    /* ================= PARSE ================= */

    int was_fixed = 0;
    int fix_penalty = 0;

    ASTNode *root = NULL;

    if (avastha->is_program_mode)
        root = parse_program(input);
    else
        root = language_execute_with_fix(input, &was_fixed, &fix_penalty);

    /* ================= ERROR ================= */

    if (!root || error_reported)
    {
        avastha->ast = NULL;
        avastha->stop_execution = 1;
        success_streak = 0;

        const ShrijiErrorInfo *err_ptr = shriji_last_error();

        if (!err_ptr)
            return 0;

        ShrijiErrorInfo err_safe = *err_ptr;

        DecisionType d_type = shriji_take_decision(
            session_confidence,
            session_risk,
            err_safe.code
        );

        char fixed[256];
        FixType ftype = fix_apply(input, fixed);

        /* AUTO FIX ONLY */
        if (ftype == FIX_SAFE && d_type == DECISION_AUTO_FIX)
        {
            printf("\n[Shriji] auto fix → %s\n\n", fixed);

            avastha->has_correction = 1;

            snprintf(avastha->corrected_text,
                     sizeof(avastha->corrected_text),
                     "%s", fixed);

            return 0;
        }

        /*  NO EI CALL HERE */
        return 0;
    }

    /* ================= SUCCESS ================= */
   if (!root || error_reported || avastha->stop_execution)
{
    return 0;
}
    avastha->ast = root;

    ShrijiRuntime runtime;
    runtime_init(&runtime);

    Value result = run_program(root, GLOBAL_ENV, &runtime);

    if (!error_reported && !runtime.error_flag)
{
        success_streak++;

        int recovery = 5 + (success_streak * 2);
        if (recovery > 25) recovery = 25;

        session_confidence += recovery;
        session_risk -= recovery;

        if (was_fixed)
        {
            session_confidence -= fix_penalty;
            session_risk += fix_penalty;
        }

        if (result.type == VAL_NUMBER)
            printf("%g\n", result.number);
        else if (result.type == VAL_STRING && result.string)
            printf("%s\n", result.string);
    }

    value_free(&result);

    if (session_confidence < 0) session_confidence = 0;
    if (session_confidence > 100) session_confidence = 100;

    if (session_risk < 4) session_risk = 4;
    if (session_risk > 100) session_risk = 100;

    KRSTDecision decision = {0};
    decision.confidence_score = session_confidence;

    apply_thresholds(&decision);

    if (DEV_MODE)
    {
        printf("[KRST] Confidence: %d | Risk: %d | Teach: %s | Tone: %s | Esc: %d\n",
               session_confidence,
               session_risk,
               teach_label(decision.teaching_level),
               tone_label(decision.tone),
               decision.escalate);
    }

    avastha->confidence = session_confidence;
    avastha->risk = session_risk;
    avastha->teach_level = decision.teaching_level;
    avastha->tone = decision.tone;

    pragya_route(avastha);

    return 0;
}
