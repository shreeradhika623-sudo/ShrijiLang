/* RADHE_RADHE_SHREEJI_RADHE_RANI */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/interpreter.h"
#include "../include/error.h"
#include "../include/fix_engine.h"
#include "../include/fix_rules.h"

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

#include "../gyaan/core/gyaan_engine.h"
#include "../include/krst_print.h"
#include "../include/krst_json.h"

extern Env *GLOBAL_ENV;

/* ================= SESSION ================= */

static int session_confidence = 100;
static int session_risk = 4;
static int success_streak = 0;

#define MAX_ERROR_TYPES 50

typedef struct {
    int code;
    int count;
} ErrorMemory;

static ErrorMemory error_memory[MAX_ERROR_TYPES];
static int error_memory_size = 0;

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

static int update_error_memory(int code)
{
    for (int i = 0; i < error_memory_size; i++)
    {
        if (error_memory[i].code == code)
        {
            error_memory[i].count++;
            return error_memory[i].count;
        }
    }

    /* 🔻 MEMORY FULL → RESET */
    if (error_memory_size >= MAX_ERROR_TYPES)
    {
        error_memory_size = 0;
    }

    error_memory[error_memory_size].code = code;
    error_memory[error_memory_size].count = 1;
    error_memory_size++;

    return 1;
}

/*──────────────────────────────────────────────
   KRST RESPONSE DECISION
──────────────────────────────────────────────*/
static KRSTResponseType krst_decide_response_type(
    int repeat,
    const FixRule *rule
)
{
    /* CRITICAL */
    if (repeat >= 10)
    {
        return KRST_RESPONSE_CRITICAL;
    }

    /* WARNING */
    if (repeat >= 7)
    {
        return KRST_RESPONSE_WARNING;
    }

    /* AUTO FIX */
    if (repeat >= 3)
    {
        if (rule != NULL)
        {
            return KRST_RESPONSE_AUTO_FIX;
        }
    }

    /* DEFAULT */
    return KRST_RESPONSE_ERROR;
}

/*──────────────────────────────────────────────
   FULL ERROR HANDLER (CENTRALIZED)
──────────────────────────────────────────────*/
static void handle_error(
    PragyaAvastha *avastha,
    const ShrijiErrorInfo *err,
    KRSTResponseType response_type
)
{

   KRSTResponse response;
memset(&response, 0, sizeof(response));

  /* MESSAGE */
snprintf(
    response.message,
    sizeof(response.message),
    "%s",
    (strlen(err->message) > 0)
        ? err->message
        : "Kuch unexpected hua hai"
);


/* INPUT */
if (err->has_location && avastha->raw_text)
{
    snprintf(
        response.input,
        sizeof(response.input),
        "%s",
        avastha->raw_text
    );

    response.pointer_col = err->col;
}

/* HINT */
if (strlen(err->hint) > 0)
{
    snprintf(
        response.hint,
        sizeof(response.hint),
        "%s",
        err->hint
    );
}



   response.type = response_type;

    /* 1. Base Error (always) */


/*  CORE INTELLIGENCE SUGGESTION */

if (strcmp(err->expected, "value") == 0)
{
    if (strcmp(err->received, "+") == 0 ||
        strcmp(err->received, "-") == 0 ||
        strcmp(err->received, "*") == 0 ||
        strcmp(err->received, "/") == 0)
    {
        snprintf(
    response.details,
    sizeof(response.details),
    "extra operator remove karo"
  );
    }
}

    /* 2. GYAAN fetch */
    const char *gyaan = gyaan_get(err);

     /* EXPLAIN LEVEL */
      if (avastha->teach_level >= KRST_TEACH_EXPLAIN)
   {
      if (gyaan &&
         strcmp(gyaan, err->message) != 0)
    {
      snprintf(
    response.details,
    sizeof(response.details),
    "%s",
    gyaan
  );
    }
    }

    /* DEEP LEVEL */
    if (avastha->teach_level >= KRST_TEACH_DEEP)
    {
        shriji_error_intelligence(avastha, err, NULL);
    }

    /* TRAIN LEVEL (future expandable) */
    if (avastha->teach_level >= KRST_TEACH_TRAIN)
    {
       snprintf(
    response.details,
    sizeof(response.details),
    "Practice karo: similar expression likho aur test karo"
  );
    }


   krst_print_response(&response);
    /* 4. Stop execution */
    avastha->stop_execution = 1;

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

EngineResult krst_process(PragyaAvastha *avastha)
{
    EngineResult res;
    engine_result_init(&res);

    if (!avastha || !avastha->raw_text)
    {
        res.status = ENGINE_PARSE_ERROR;
        return res;
    }

    const char *input = avastha->raw_text;

    /* NIRMAN START */
    if (strcmp(input, "nirman") == 0) {
        nirman_start();
        printf("Nirman mode activated\n");
        printf("Welcome to Shriji World\n");
        res.status = ENGINE_OK;
        return res;
    }

    /* NIRMAN STOP */
    if (strcmp(input, "exit nirman") == 0) {
        nirman_stop();
        printf("Nirman mode deactivated\n");
        res.status = ENGINE_OK;
        return res;
    }

    /* NIRMAN MODE */
    if (nirman_is_active()) {
        int intent = nirman_detect_intent(input);
        printf("SHRIJI (intent=%d)\n", intent);
        res.status = ENGINE_OK;
        return res;
    }

    if (DEV_MODE)
        printf("[KRST] Input: %s\n", input);

    error_reported = 0;
    avastha->stop_execution = 0;
    avastha->allow_auto_fix = 0;

/* 🔻 RUN ENGINE */
EngineResult engine_res =
    shriji_engine_execute(input, GLOBAL_ENV);

/* 🔻 ERROR HANDLE */
const ShrijiErrorInfo *err_ptr = NULL;

if (engine_res.status != ENGINE_OK)
{
    err_ptr = shriji_last_error();
}

if (err_ptr)
{

    ShrijiErrorInfo err_safe = *err_ptr;
    error_reported = 0;

int repeat = update_error_memory(err_safe.code);

/* FIX RULE LOOKUP */
FixRule *rule =
    shriji_get_rule_for_error(err_safe.code);

/* RESPONSE TYPE DECISION */
KRSTResponseType response_type =
    krst_decide_response_type(repeat, rule);

/* 🔻 ADD THIS BLOCK BELOW */
session_confidence -= 10;

if (session_confidence < 0)
    session_confidence = 0;

switch (response_type)
{
    case KRST_RESPONSE_ERROR:

        avastha->teach_level = KRST_TEACH_EXPLAIN;
        avastha->risk = 20;
        avastha->allow_auto_fix = 0;

        break;

    case KRST_RESPONSE_WARNING:

        avastha->teach_level = KRST_TEACH_DEEP;
        avastha->risk = 50;
        avastha->allow_auto_fix = 0;

        break;

    case KRST_RESPONSE_AUTO_FIX:

        avastha->teach_level = KRST_TEACH_DEEP;
        avastha->risk = 50;
        avastha->allow_auto_fix = 1;

        break;

    case KRST_RESPONSE_CRITICAL:

        avastha->teach_level = KRST_TEACH_TRAIN;
        avastha->risk = 80;
        avastha->allow_auto_fix = 0;

        break;

    default:
        break;
}

    /* CENTRAL ERROR HANDLER */
    handle_error(
     avastha,
     &err_safe,
     response_type
   );

    /*  FIX ENGINE RE-EXECUTION */
if (avastha->has_correction && avastha->allow_auto_fix)
{
    if (DEV_MODE)
    {
        printf("[KRST] Applying auto-correction...\n");
    }

    EngineResult final_res = shriji_engine_execute(
        avastha->corrected_text,
        GLOBAL_ENV
    );

    /*  APPLY FIXED RESULT */
    if (final_res.status == ENGINE_OK)
    {
        value_free(&engine_res.result);
        engine_res.result = value_copy(final_res.result);

        value_free(&final_res.result);
        engine_res.status = ENGINE_OK;
        avastha->stop_execution = 0;  // allow normal flow
    }
    else
    {
        avastha->stop_execution = 1;
    }
}

}
/* 🟢 SUCCESS TRACKING */
if (engine_res.status == ENGINE_OK)
{
    success_streak++;

    /* 🔻 RESET ONLY AFTER STRONG PROOF */
    if (success_streak >= 5)
    {
        error_memory_size = 0;
        session_confidence = 100;

        avastha->risk = 5;
        avastha->teach_level = KRST_TEACH_SILENT;
    }
}
else
{
    /* 🔻 DON'T HARD RESET */
    if (success_streak > 0)
        success_streak--;
}

if (engine_res.status == ENGINE_OK)
{
    /* AUTO FIX */
    if (avastha->allow_auto_fix)
    {
          KRSTResponse response;
    memset(&response, 0, sizeof(response));

      response.type = KRST_RESPONSE_AUTO_FIX;

        snprintf(
        response.original_input,
        sizeof(response.original_input),
        "%s",
        avastha->raw_text
    );

        snprintf(
        response.corrected_input,
        sizeof(response.corrected_input),
        "%s",
        avastha->corrected_text
    );

         if (engine_res.result.type == VAL_NUMBER)
    {
        snprintf(
            response.result_text,
            sizeof(response.result_text),
            "%g",
            engine_res.result.number
        );
    }
    else if (
        engine_res.result.type == VAL_STRING &&
        engine_res.result.string
    )
    {
        snprintf(
            response.result_text,
            sizeof(response.result_text),
            "%s",
            engine_res.result.string
        );
    }

    krst_print_response(&response);

        return engine_res;
    }

    /* NORMAL OUTPUT */
if (engine_res.status == ENGINE_OK &&
    current_runtime &&
    current_runtime->last_output_mode != OUTPUT_EXPLICIT)
{
    return engine_res;
}

KRSTResponse response;

memset(&response, 0, sizeof(response));

response.type = KRST_RESPONSE_NORMAL;


if (engine_res.result.type == VAL_NUMBER)
{
    snprintf(
        response.result_text,
        sizeof(response.result_text),
        "%g",
        engine_res.result.number
    );
}
else if (
    engine_res.result.type == VAL_STRING &&
    engine_res.result.string
)
{
    snprintf(
        response.result_text,
        sizeof(response.result_text),
        "%s",
        engine_res.result.string
    );
}
else if (engine_res.result.type == VAL_BOOL)
{
    snprintf(
        response.result_text,
        sizeof(response.result_text),
        "%s",
        engine_res.result.boolean ? "true" : "false"
    );
}
else
{
    snprintf(
        response.result_text,
        sizeof(response.result_text),
        "null"
    );
}

snprintf(
    response.explain_text,
    sizeof(response.explain_text),
    "%s",
    engine_res.explain_text
);

krst_print_response(&response);

char json[4096];

krst_response_to_json(
    &response,
    json,
    sizeof(json)
);

printf("\nJSON:\n%s\n", json);

}
return engine_res;
}
