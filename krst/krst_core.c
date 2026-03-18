/* RADHE_RADHE_SHREEJI_RADHE_RANI */

#include <stdio.h>
#include <string.h>

#include "../include/interpreter.h"
#include "../include/error.h"
#include "../include/fix_engine.h"
#include "../include/value.h"
#include "../include/env.h"
#include "../include/runtime.h"
#include "../include/pragya_avastha.h"

#include "krst_core.h"
#include "krst_types.h"

#include "../pragya/router/l3_router.h"
#include "../pragya/contracts/l3_request.h"
#include "../include/error_intelligence.h"

extern Env *GLOBAL_ENV;

/* ========================================================= */
/* SESSION MEMORY                                            */
/* ========================================================= */

static int session_confidence = 100;
static int session_risk = 4;
static int repeat_error_count = 0;
static ShrijiErrorCode last_error_code = E_UNKNOWN;

/* ========================================================= */
/* LABEL HELPERS                                             */
/* ========================================================= */

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

/* ========================================================= */
/* ERROR SEVERITY                                            */
/* ========================================================= */

static int error_severity(ShrijiErrorCode code)
{
    switch(code)
    {
        case E_RUNTIME_DIV_ZERO:      return 4;
        case E_RUNTIME_TYPE_MISMATCH: return 3;
        case E_RUNTIME_LOOP_LIMIT:    return 5;
        case E_PARSE_01:
        case E_PARSE_02:              return 2;
        default:                      return 2;
    }
}

/* ========================================================= */
/* THRESHOLD ENGINE                                          */
/* ========================================================= */

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

/* ========================================================= */
/* MAIN PROCESS                                              */
/* ========================================================= */

int krst_process(const KRSTRequest *req)
{
    if (!req || !req->input_text)
        return 1;

    const char *input = req->input_text;

    printf("[KRST] Input: %s\n", input);

    KRSTDecision decision;
    decision.confidence_score = session_confidence;
    decision.teaching_level = KRST_TEACH_SILENT;
    decision.tone = KRST_TONE_NEUTRAL;
    decision.escalate = 0;

    error_reported = 0;

    /* ================= PragyaAvastha ================= */

    PragyaAvastha avastha;
    memset(&avastha, 0, sizeof(PragyaAvastha));

    avastha.raw_text = input;
    avastha.confidence = session_confidence;
    avastha.risk = session_risk;

    /* ================= PARSE + FIX ================= */

    int was_fixed = 0;
    int fix_penalty = 0;

    ASTNode *root = language_execute_with_fix(
        input,
        &was_fixed,
        &fix_penalty
    );

    avastha.ast = root;

    /* DEBUG */

    if (root) {
        printf("[KRST] ROOT Type: %d\n", root->type);

        if (root->type == AST_PROGRAM && root->stmt_count > 0) {
            printf("[KRST] FIRST STMT Type: %d\n",
                   root->statements[0]->type);
        }
    }

    if (root) {
        printf("[KRST] AST Type: %d\n", root->type);
    }

    /* ================= PARSE FAILURE ================= */

    if (!root || error_reported)
    {
        const ShrijiErrorInfo *err = shriji_last_error();

        if (err)
            avastha.error_code = err->code;
       shriji_error_intelligence(&avastha, err, NULL);

        if (avastha.stop_execution)
             return 0;

        if (err && err->code == last_error_code)
            repeat_error_count++;
        else
            repeat_error_count = 1;

        if (repeat_error_count > 5)
            repeat_error_count = 5;

        last_error_code = err ? err->code : E_UNKNOWN;

        int sev = error_severity(last_error_code);
        int penalty = (sev * 10) + (repeat_error_count * 3);

        session_confidence -= penalty;
        session_risk += penalty;

        session_confidence -= fix_penalty;
        session_risk += fix_penalty;
    }
    else
    {
        /* ================= RUNTIME ================= */

        ShrijiRuntime runtime;
        runtime_init(&runtime);

        Value result = run_program(root, GLOBAL_ENV, &runtime);

        avastha.runtime_result = result;

        if (error_reported)
        {
            const ShrijiErrorInfo *err = shriji_last_error();

            if (err)
                avastha.error_code = err->code;

            if (err && err->code == last_error_code)
                repeat_error_count++;
            else
                repeat_error_count = 1;

            if (repeat_error_count > 5)
                repeat_error_count = 5;

            last_error_code = err ? err->code : E_UNKNOWN;

            int sev = error_severity(last_error_code);
            int penalty = (sev * 10) + (repeat_error_count * 3);

            session_confidence -= penalty;
            session_risk += penalty;
        }
        else
        {
            repeat_error_count = 0;
            last_error_code = E_UNKNOWN;

            if (was_fixed)
            {
                session_confidence -= fix_penalty;
                session_risk += fix_penalty;
            }

            session_confidence += 6;
            session_risk -= 5;

            if (result.type == VAL_NUMBER)
                printf("%g\n", result.number);
            else if (result.type == VAL_STRING && result.string)
                printf("%s\n", result.string);

            value_free(&result);
        }
    }

    /* ================= HARD BOUNDS ================= */

    if (session_confidence < 0)
        session_confidence = 0;

    if (session_confidence > 100)
        session_confidence = 100;

    if (session_risk < 4)
        session_risk = 4;

    if (session_risk > 100)
        session_risk = 100;

    decision.confidence_score = session_confidence;

    apply_thresholds(&decision);

    printf("[KRST] Confidence: %d | Risk: %d | Teach: %s | Tone: %s | Esc: %d\n",
           session_confidence,
           session_risk,
           teach_label(decision.teaching_level),
           tone_label(decision.tone),
           decision.escalate);

/* ===== Pragya call ===== */

/* User input */
avastha.raw_text = input;

/* Language state */
avastha.ast = root;

/* Error state */
avastha.error_code = last_error_code;

/* KRST metrics */
avastha.confidence = session_confidence;
avastha.risk = session_risk;

avastha.teach_level = decision.teaching_level;
avastha.tone = decision.tone;

/* Runtime result */
memset(&avastha.runtime_result, 0, sizeof(Value));

/* Send to Pragya */
pragya_route(&avastha);

return 0;
}
