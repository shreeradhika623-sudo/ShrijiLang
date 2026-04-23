#include <stdio.h>
#include <string.h>

#include "../include/krst_router.h"
#include "../krst/krst_core.h"
#include "../include/pragya_avastha.h"

#include "../include/error.h"
#include "../include/error_intelligence.h"

/* ===============================
   INPUT NORMALIZATION (SAFE)
   =============================== */
static void normalize_input(const char *input, char *output)
{
    if (!input || !output) return;

    int i = 0;
    for (; input[i]; i++) {
        if (input[i] >= 'A' && input[i] <= 'Z')
            output[i] = input[i] + 32;
        else
            output[i] = input[i];
    }
    output[i] = '\0';
}

/* ===============================
   COMMAND DETECTION
   =============================== */
static char detect_command(const char *input)
{
    if (!input) return 'G';

    if (input[0] == 'g' && input[1] == ' ')
        return 'G';

    if (input[0] == 'e' && input[1] == ' ')
        return 'E';

    return 'G';
}

/* ===============================
   CONTEXT REWRITE (C2 COMPLETE)
   =============================== */
static int rewrite_context_input(KRSTRequest *req)
{
    if (!req || !req->input_text) return 0;

    int num = 0;
    static char buffer[128];

    const char *input = req->input_text;

    /* ===== ADD ===== */
    if ((strstr(input, "add") || strstr(input, "jodo")) &&
        sscanf(input, "usme %d", &num) == 1) {

        snprintf(buffer, sizeof(buffer), "bolo last + %d", num);
        req->input_text = buffer;
        return 1;
    }

    /* ===== MINUS ===== */
    if ((strstr(input, "minus") || strstr(input, "ghatao") || strstr(input, "subtract")) &&
        (sscanf(input, "usme %d", &num) == 1 ||
         sscanf(input, "usme se %d", &num) == 1)) {

        snprintf(buffer, sizeof(buffer), "bolo last - %d", num);
        req->input_text = buffer;
        return 1;
    }

    /* ===== MULTIPLY ===== */
    if ((strstr(input, "multiply") || strstr(input, "guna") || strstr(input, "into")) &&
        sscanf(input, "usme %d", &num) == 1) {

        snprintf(buffer, sizeof(buffer), "bolo last * %d", num);
        req->input_text = buffer;
        return 1;
    }

    /* ===== DIVIDE ===== */
    if ((strstr(input, "divide") || strstr(input, "bhaag") || strstr(input, "divided")) &&
        (sscanf(input, "usme %d", &num) == 1 ||
         sscanf(input, "usme se %d", &num) == 1)) {

        snprintf(buffer, sizeof(buffer), "bolo last / %d", num);
        req->input_text = buffer;
        return 1;
    }

    return 0;
}

/* ===============================
   MAIN ROUTER
   =============================== */
void krst_route_request(KRSTRequest *req)
{
    if (!req || !req->input_text)
        return;

    /* ===== SAFE NORMALIZATION ===== */
    static char normalized[256];

    normalize_input(req->input_text, normalized);
    req->input_text = normalized;

    /* ===== COMMAND DETECTION ===== */
    char cmd = detect_command(req->input_text);

    /* ===== COMMAND ROUTING ===== */

     if (cmd == 'E') {
    req->input_text += 2;

    static char explain_buf[256];
   snprintf(explain_buf, sizeof(explain_buf), "bolo %.240s", req->input_text);
    req->input_text = explain_buf;

    printf("[EXPLAIN MODE]\n");
}

    else if (cmd == 'G') {
        if (req->input_text[0] == 'g' && req->input_text[1] == ' ')
            req->input_text += 2;
    }

    /* ===== CONTEXT REWRITE ===== */
    rewrite_context_input(req);

    /* ===== CREATE AVASTHA ===== */
    PragyaAvastha avastha;
    memset(&avastha, 0, sizeof(PragyaAvastha));

    avastha.raw_text = req->input_text;

      avastha.raw_text = req->input_text;

/*  NEW: pass input mode */
    avastha.is_program_mode = req->is_program_mode;

/* ===== KRST CORE ===== */
krst_process(&avastha);

/* ===== ERROR INTELLIGENCE FALLBACK ===== */
if (!avastha.has_correction && error_reported)
{
    shriji_error_intelligence(&avastha, &LAST_ERROR, NULL);
}

/* ===== RETURN CORRECTION ===== */
if (avastha.has_correction)
{
    req->has_correction = 1;

    strncpy(req->corrected_text,
            avastha.corrected_text,
            sizeof(req->corrected_text) - 1);

    req->corrected_text[
        sizeof(req->corrected_text) - 1] = '\0';
}
}
