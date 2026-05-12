#ifndef KRST_TYPES_H
#define KRST_TYPES_H

#include "../include/env.h"

/* ==============================
   KRST ACTION ENUM
============================== */

typedef enum {
    KRST_ACTION_ALLOW = 0,
    KRST_ACTION_BLOCK,
    KRST_ACTION_TEACH,
    KRST_ACTION_ESCALATE
} KRSTAction;

/* ==============================
   KRST TEACHING LEVEL ENUM
============================== */

typedef enum {
    KRST_TEACH_SILENT = 0,
    KRST_TEACH_HINT,
    KRST_TEACH_EXPLAIN,
    KRST_TEACH_DEEP,
    KRST_TEACH_TRAIN
} KRSTTeachingLevel;

/* ==============================
   KRST TONE ENUM
============================== */

typedef enum {
    KRST_TONE_NEUTRAL = 0,
    KRST_TONE_SUPPORTIVE,
    KRST_TONE_GUIDING,
    KRST_TONE_STRICT,
    KRST_TONE_ALERT
} KRSTTone;

/* ==============================
   KRST REQUEST STRUCT
============================== */
typedef struct
{
    const char *input_text;

     int is_program_mode;
    /* correction support */

    char corrected_text[512];
    int has_correction;
     int stop_execution;

} KRSTRequest;
/* ==============================
   KRST DECISION STRUCT
============================== */

typedef struct {
    KRSTAction action;

    int confidence_score;   /* 0–100 */
    int risk_score;         /* 0–100 */

    int allow_execution;
    int reason_code;

    KRSTTeachingLevel teaching_level;
    KRSTTone tone;

    int escalate;   /* 1 = escalation required */

} KRSTDecision;

/* ==============================
   KRST RESPONSE TYPE
============================== */

typedef enum {
    KRST_RESPONSE_NORMAL = 0,
    KRST_RESPONSE_ERROR,
    KRST_RESPONSE_WARNING,
    KRST_RESPONSE_AUTO_FIX,
    KRST_RESPONSE_CRITICAL
} KRSTResponseType;

/* ==============================
   KRST RESPONSE STRUCT
============================== */

typedef struct {

    KRSTResponseType type;

    char message[256];
    char details[512];
    char cause[256];

    char input[512];
    int pointer_col;

    char hint[256];

    char result_text[256];

    char explain_text[2048];

    char original_input[512];
    char corrected_input[512];

    int repeat_count;

} KRSTResponse;

#endif
