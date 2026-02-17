#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/interpreter.h"
#include "../include/parser.h"
#include "../include/env.h"
#include "../include/value.h"
#include "../include/smriti.h"
#include "../include/error.h"

/* ───────────────────────────────────────── */
/* Local teacher-side eval (STEP-19)        */
/* Mira se independent                      */
/* ───────────────────────────────────────── */

static int teacher_is_op(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

static int teacher_eval(const char *expr, double *out)
{
    double result = 0.0;
    double current = 0.0;
    char op = '+';

    const char *p = expr;

    while (*p)
    {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }

        if (!isdigit((unsigned char)*p) && *p != '.') {
            return 0;
        }

        char *end;
        current = strtod(p, &end);
        p = end;

        switch (op)
        {
            case '+': result += current; break;
            case '-': result -= current; break;
            case '*': result *= current; break;
            case '/':
                if (current == 0) return 0;
                result /= current;
                break;
            default:
                return 0;
        }

        while (isspace((unsigned char)*p)) p++;

        if (*p && teacher_is_op(*p)) {
            op = *p;
            p++;
        }
    }

    *out = result;
    return 1;
}


/* ───────────────────────────────────────── */

Env *GLOBAL_ENV;

/* helpers */

static int is_number(const char *s)
{
    if (!s || !*s) return 0;
    char *end;
    strtod(s, &end);
    return (*end == '\0');
}

static int is_unary_number(const char *s)
{
    if (!s || !*s) return 0;
    if (*s == '+' || *s == '-')
        return is_number(s + 1);
    return is_number(s);
}

static int is_trailing_op(const char *s)
{
    if (!s) return 0;
    int i = (int)strlen(s) - 1;
    while (i >= 0 && isspace((unsigned char)s[i])) i--;
    if (i < 0) return 0;
    return (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/');
}

/* ───────────────────────────────────────── */

static void interactive_shell(void)
{
    char line[512];

    printf("──────────────────────────────────────────────\n");
    printf("        🌸 ShrijiLang Interactive Shell 🌸\n");
    printf("──────────────────────────────────────────────\n");
    printf("Type 'exit' to quit\n\n");

    while (1)
    {
        printf("Shiri> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "exit") == 0) {
            printf("Radhe Radhe 🌸\n");
            break;
        }

        if (line[0] == '\0')
            continue;
        /* ── STEP-17: Post-error decision gate ── */
        extern int error_reported;
        if (error_reported)
        {
            printf("[G] Guide    [X] Exit\n");

            int ch = getchar();
            while (ch == '\n')
                ch = getchar();   /* newline skip */

            if (ch == 'X' || ch == 'x') {
                printf("Radhe Radhe 🌸\n");
                break;
            }

            if (ch == 'G' || ch == 'g') {
                const ShrijiErrorInfo *e = shriji_last_error();

                if (!e) {
                    printf("Abhi koi error context available nahi hai.\n");
                } else {
                    printf("\n— Guide —\n");

                    /* 1) What happened */
                    printf("Kya hua:\n  %s\n", e->message ? e->message : "Unknown error");

                    /* 2) Where */
                    if (e->has_location) {
                        printf("Kahan:\n  line %d, col %d\n", e->line, e->col);
                    }

                    /* 3) Context (short) */
                    if (e->context && e->context[0]) {
                        printf("Context:\n  %s\n", e->context);
                    }

                    /* 4) Hint (how to fix) */
                    if (e->hint && e->hint[0]) {
                        printf("Kaise theek karein:\n  %s\n", e->hint);
                    }

                    printf("\nControl tumhare haath me hai. Fix karke dobara try karo.\n");
                }
            }

            error_reported = 0;
            continue;
        }

        /* ── CASE 1: trailing operator → teaching + continuation ── */
        if (is_trailing_op(line))
        {
            int i = (int)strlen(line) - 1;
            while (i >= 0 && isspace((unsigned char)line[i])) i--;

            char op = line[i];
            line[i] = '\0';

            double partial = 0.0;
            if (teacher_eval(line, &partial))
            {
                printf(
                    "Tumne yeh likha:\n"
                    "  %s\n\n"
                    "Iska partial result hota hai:\n"
                    "  %.10g\n\n"
                    "Lekin last me operator ke baad number missing hai.\n"
                    "Kya tum aage aur add / subtract / multiply karna chahte ho?\n",
                    line, partial
                );

                smriti_set_continuation(partial, op);
            }
            continue;
        }

        /* ── CASE 2: continuation + unary * / ── */
        if (smriti_has_continuation() &&
            (line[0] == '*' || line[0] == '/'))
        {
            char expr[512];
            snprintf(
                expr, sizeof(expr),
                "%.10g%c%.400s",
                smriti_get_continuation_value(),
                line[0],
                line + 1
            );

            smriti_clear_continuation();

            ASTNode *root = parse_program(expr);
            if (!root) continue;

            Value v = eval(root, GLOBAL_ENV);
            if (v.type == VAL_NUMBER)
                printf("%.*g\n", 12, v.number);

            value_free(&v);
            ast_free(root);
            continue;
        }

        /* ── CASE 3: unary + / - number ── */
        if (is_unary_number(line))
        {
            ASTNode *root = parse_program(line);
            if (!root) continue;

            Value v = eval(root, GLOBAL_ENV);
            if (v.type == VAL_NUMBER)
                printf("%.*g\n", 12, v.number);

            value_free(&v);
            ast_free(root);
            continue;
        }

        /* ── DEFAULT: normal expression ── */
        ASTNode *root = parse_program(line);
        if (!root)
        {
            printf("Ye input mujhe clear nahi laga. Thoda simple likhne ki koshish karo.\n");
            continue;
        }

        Value v = eval(root, GLOBAL_ENV);
        if (v.type == VAL_NUMBER)
            printf("%.*g\n", 12, v.number);

        value_free(&v);
        ast_free(root);
    }
}

/* ───────────────────────────────────────── */

int main(void)
{
    GLOBAL_ENV = new_env();
    smriti_init();
    interactive_shell();
    return 0;
}
