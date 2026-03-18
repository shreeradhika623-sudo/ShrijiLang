/* SHREE_RADHIKA_RANI */

#include <stdio.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/error.h"
#include "../include/fix_engine.h"
#include "../include/fix_rules.h"

#define MAX_WORKING_BUFFER 512


ASTNode *language_execute_with_fix(
    const char *input,
    int *was_fixed,
    int *penalty_out
)
{
    if (!input)
        return NULL;

    if (was_fixed)
        *was_fixed = 0;

    if (penalty_out)
        *penalty_out = 0;


    char working_buffer[MAX_WORKING_BUFFER];

    strncpy(
        working_buffer,
        input,
        MAX_WORKING_BUFFER - 1
    );

    working_buffer[MAX_WORKING_BUFFER - 1] = '\0';


    int attempt = 0;


    while (1)
    {
        attempt++;


        /* =========================
           PARSE
        ========================= */

        error_reported = 0;

        shriji_set_error_mode(ERROR_MODE_IMMEDIATE);

        ASTNode *root =
            parse_program(working_buffer);


        /* =========================
           SUCCESS
        ========================= */

        if (!error_reported && root)
            return root;


        /* =========================
           FETCH ERROR
        ========================= */

        const ShrijiErrorInfo *err =
            shriji_last_error();

        if (!err)
            return root;


        /* =========================
           FIND RULE
        ========================= */

        FixRule *rule =
            shriji_get_rule_for_error(err->code);

        if (!rule)
            return root;


        /* =========================
           MAX ATTEMPT
        ========================= */

        if (attempt > rule->max_attempt)
            return root;


        /* =========================
           APPLY FIX
        ========================= */

        int fixed =
            rule->apply_fix(
                working_buffer,
                sizeof(working_buffer),
                err
            );

        if (!fixed)
            return root;


        /* =========================
           MARK FIX
        ========================= */

        if (was_fixed)
            *was_fixed = 1;

        if (penalty_out)
            *penalty_out += rule->confidence_penalty;


        printf(
            "\n[Shriji] expression updated → %s\n\n",
            working_buffer
        );


        /* =========================
           REPARSE CONTROL
        ========================= */

        if (!rule->reparse_required)
            return root;

        /* loop again for reparse */
    }
}
