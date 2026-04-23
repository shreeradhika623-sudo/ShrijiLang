#include <stdio.h>
#include <string.h>

#include "../include/fix_interactive.h"
#include "../include/error.h"

/* =========================================================
   MEMORY — LAST INPUT TRACKING
========================================================= */

static char last_input[512] = {0};

/* =========================================================
   SAFE LINE INPUT
========================================================= */

static int read_line(char *buffer, int size)
{
    if (!fgets(buffer, size, stdin))
        return 0;

    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

/* =========================================================
   SAME INPUT GUARD
========================================================= */

static int is_same_input(const char *input)
{
    if (strcmp(last_input, input) == 0)
    {
        printf("\n[SYSTEM] Same input detected. Please modify it.\n");
        return 1;
    }

    strncpy(last_input, input, sizeof(last_input) - 1);
    last_input[sizeof(last_input) - 1] = '\0';

    return 0;
}

/* =========================================================
   FIX MENU CORE (SHARED)
========================================================= */

static int show_fix_menu(const char *input, char *output, int size)
{
    while (1)
    {
        printf("\nFix Options:\n\n");
        printf("M) Manual edit\n");
        printf("E) Quick edit\n");
        printf("I) Ignore\n");

        printf("\nSelect: ");

        char choice = getchar();
        getchar(); /* consume newline */

        /* ========================
           M — Manual Edit
        ======================== */
        if (choice == 'M' || choice == 'm')
        {
            printf("Enter new expression: ");

            if (!read_line(output, size))
                return 0;

            if (is_same_input(output))
                continue;

            return 1;
        }

        /* ========================
           E — Quick Edit
        ======================== */
        if (choice == 'E' || choice == 'e')
        {
            printf("Edit:\n%s\n> ", input);

            if (!read_line(output, size))
                return 0;

            if (is_same_input(output))
                continue;

            return 1;
        }

        /* ========================
           I — Ignore
        ======================== */
        if (choice == 'I' || choice == 'i')
        {
            return 0;
        }

        printf("Invalid choice. Try again.\n");
    }
}

/* =========================================================
   PUBLIC WRAPPERS
========================================================= */

int shriji_interactive_double_operator_fix(
    const char *input,
    char *output,
    int size)
{
    return show_fix_menu(input, output, size);
}

int shriji_interactive_bracket_fix(
    const char *input,
    char *output,
    int size)
{
    return show_fix_menu(input, output, size);
}

int shriji_interactive_operator_fix(
    const char *input,
    char *output,
    int size)
{
    return show_fix_menu(input, output, size);
}
