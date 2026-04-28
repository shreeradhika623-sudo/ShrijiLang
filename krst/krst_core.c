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
   COMMON ERROR PRINT (PARSE + RUNTIME)
──────────────────────────────────────────────*/
static void print_shriji_error(
    PragyaAvastha *avastha,
    const ShrijiErrorInfo *err
)
{
    printf("\n");

    /* ===== SHRIJI TONE ===== */
    if (avastha->risk >= 76)
    {
        printf("🌺 Thoda rukte hain...\n");
        printf("yahan dhyaan dene ki zarurat hai\n");
        printf("chalo isko sahi tarah se samajhte hain\n\n");
    }
    else if (avastha->risk >= 30)
    {
        printf("🌼 Lagta hai yahan ek pattern repeat ho raha hai\n");
        printf("chalo isko dhyaan se samajhte hain\n\n");
    }
    else
    {
        printf("🌸 Thoda sa issue aaya hai, dekhte hain saath mein\n\n");
    }

    /* ===== MAIN ERROR ===== */
    printf("❌ %s\n",
        err->message ? err->message : "Kuch unexpected hua hai");

    if (err->hint && strlen(err->hint) > 2)
        printf("👉 %s\n", err->hint);

    if (err->has_location)
        printf("(line %d, col %d)\n", err->line, err->col);

    /* ===== HUMAN EXPLANATION ===== */
    if (err->has_location && avastha->raw_text)
    {
        int pos = err->col > 0 ? err->col - 1 : 0;
        int len = strlen(avastha->raw_text);

        if (pos >= 0 && pos < len)
        {
            char wrong_char = avastha->raw_text[pos];

            if (isprint(wrong_char))
            {
                if (isalpha(wrong_char))
                    printf("💡 '%c' yahan valid nahi lag raha\n\n", wrong_char);
                else if (isdigit(wrong_char))
                    printf("💡 yahan number ke baad kuch missing lag raha hai\n\n");
                else if (strchr("+-*/", wrong_char))
                    printf("💡 operator ka use galat ho gaya hai\n\n");
            }
        }
    }

    printf("\n");
}

/*──────────────────────────────────────────────
   FULL ERROR HANDLER (CENTRALIZED)
──────────────────────────────────────────────*/
static void handle_error(
    PragyaAvastha *avastha,
    const ShrijiErrorInfo *err,
    Value *result   /* NULL for parse error */
)
{
    print_shriji_error(avastha, err);

    shriji_error_intelligence(avastha, err, NULL);

    avastha->stop_execution = 1;

    if (result != NULL)
        value_free(result);
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

    /* NIRMAN START */
    if (strcmp(input, "nirman") == 0) {
        nirman_start();
        printf("Nirman mode activated\n");
        printf("Welcome to Shriji World\n");
        return 0;
    }

    /* NIRMAN STOP */
    if (strcmp(input, "exit nirman") == 0) {
        nirman_stop();
        printf("Nirman mode deactivated\n");
        return 0;
    }

    /* NIRMAN MODE */
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

char final_input[512];

ASTNode *root = language_execute_with_fix(
    input,
    final_input,
    &was_fixed,
    &fix_penalty
);
    /* ================= PARSE ERROR ================= */

    if (!root || error_reported)
    {

        avastha->ast = NULL;
        avastha->stop_execution = 1;
        success_streak = 0;

        const ShrijiErrorInfo *err_ptr = shriji_last_error();
        if (!err_ptr) return 0;

    int repeat_count = update_error_memory((int)err_ptr->code);

  /* 🔻 adaptive penalty */
   int penalty = 10 + (repeat_count - 1) * 5;
      if (penalty > 25) penalty = 25;

         session_confidence -= penalty;
         session_risk += penalty;

      if (DEV_MODE)
  {
        printf("[KRST] Error repeat count: %d\n", repeat_count);
    }

        ShrijiErrorInfo err_safe = *err_ptr;

        DecisionType d_type = shriji_take_decision(
            session_confidence,
            session_risk,
            err_safe.code
        );

        char fixed[256];
        FixType ftype = fix_apply(input, fixed);

        if (ftype == FIX_SAFE && d_type == DECISION_AUTO_FIX)
        {
          printf("Input: %s\n", input);
          printf("Correction: %s\n", fixed);

            avastha->has_correction = 1;

            snprintf(avastha->corrected_text,
                     sizeof(avastha->corrected_text),
                     "%s", fixed);
                error_reported = 0;
            return 0;
        }
               handle_error(avastha, &err_safe, NULL);
   /* 🔻 HARD BOUNDS */

if (session_confidence < 0)
{
    session_confidence = 0;
}

if (session_confidence > 100)
{
    session_confidence = 100;
}

if (session_risk < 4)
{
    session_risk = 4;
}

if (session_risk > 100)
{
    session_risk = 100;
}

      /* 🔻 ADD HERE */
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

         return 0;
    }

    /* ================= SUCCESS START ================= */

    avastha->ast = root;

    ShrijiRuntime runtime;
    runtime_init(&runtime);

    Value result = eval(root, GLOBAL_ENV, &runtime);

/* ================= RUNTIME ERROR (NEW) ================= */
if (runtime.error_flag || error_reported)
{
    success_streak = 0;

    const ShrijiErrorInfo *err_ptr = shriji_last_error();

    int repeat_count = 1;

       if (err_ptr)
 {
       repeat_count = update_error_memory((int)err_ptr->code);
  }

    /* 🔻 adaptive penalty */
    int penalty = 10 + (repeat_count - 1) * 5;
    if (penalty > 25) penalty = 25;

    session_confidence -= penalty;
    session_risk += penalty;

      if (DEV_MODE && err_ptr)
{
      printf("[KRST] Error repeat count: %d\n", repeat_count);
 }

    if (err_ptr)
    {
        ShrijiErrorInfo err_safe = *err_ptr;
        error_reported = 0;

        handle_error(avastha, &err_safe, &result);
    }
    else
    {
        avastha->stop_execution = 1;
        value_free(&result);
    }

   /* 🔻 HARD BOUNDS */

if (session_confidence < 0)
{
    session_confidence = 0;
}

if (session_confidence > 100)
{
    session_confidence = 100;
}

if (session_risk < 4)
{
    session_risk = 4;
}

if (session_risk > 100)
{
    session_risk = 100;
}

    /* 🔻 KRST PRINT */
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

    return 0;
}

/* ================= SUCCESS ================= */

   success_streak++;

  /* RESET ERROR MEMORY */
 if (success_streak >= 3)
{
    error_memory_size = 0;
}

 int recovery = 3 + (success_streak * 2);

   if (recovery > 15)
 {
     recovery = 15;
  }

   if (session_confidence > 80)
 {
     recovery = recovery / 2;
  }

   if (session_risk > 60)
 {
     recovery = recovery / 2;
  }

    session_confidence += recovery;
    session_risk -= recovery;

if (was_fixed)
{
    printf("Input: %s\n", input);
    printf("Correction: %s\n", final_input);

   session_confidence -= fix_penalty;
   session_risk += fix_penalty;
}

     if (result.type == VAL_NUMBER)
 {
     printf("%g\n", result.number);
   }
     else if (result.type == VAL_STRING && result.string)
 {
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
