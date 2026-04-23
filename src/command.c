#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../include/command.h"
#include "../include/event.h"

/*──────────────────────────────────────────────────────────────
 | COMMAND ALIAS TABLE
 | 108 public + remaining locked (sample shown)
 *──────────────────────────────────────────────────────────────*/
static CommandAlias COMMAND_ALIASES[] = {

    /* LIST */
    { "ls",        CMD_LIST,   CMD_PUBLIC },
    { "list",      CMD_LIST,   CMD_PUBLIC },
    { "radha",     CMD_LIST,   CMD_PUBLIC },
    { "r",         CMD_LIST,   CMD_PUBLIC },

    /* CD */
    { "cd",        CMD_CD,     CMD_PUBLIC },
    { "chalo",     CMD_CD,     CMD_PUBLIC },

    /* MKDIR */
    { "mkdir",     CMD_MKDIR,  CMD_PUBLIC },
    { "bana",      CMD_MKDIR,  CMD_PUBLIC },

    /* BOLO (PRINT) */
    { "bolo",      CMD_BOLO,   CMD_PUBLIC },

    /* SAMAY (TIME) ✅ */
    { "samay",     CMD_SAMAY,  CMD_PUBLIC },
    { "time",      CMD_SAMAY,  CMD_PUBLIC },
    { "clock",     CMD_SAMAY,  CMD_PUBLIC },

/* AI Modules */
    { "sakhi",     CMD_SAKHI,  CMD_PUBLIC },
    { "niyu",      CMD_NIYU,   CMD_PUBLIC },
    { "mira",      CMD_MIRA,   CMD_PUBLIC },
    { "kavya",     CMD_KAVYA,  CMD_PUBLIC },
    { "shiri",     CMD_SHIRI,  CMD_PUBLIC },

    /* LOCKED / RESERVED */
    { "shyam",     CMD_DELETE, CMD_LOCKED },
    { "keshav",    CMD_COPY,   CMD_LOCKED },

    { NULL, CMD_UNKNOWN, CMD_LOCKED }
};

/*──────────────────────────────────────────────────────────────
 | Resolve user-facing name → internal command
 *──────────────────────────────────────────────────────────────*/
CommandType resolve_command(const char *name)
{
    if (!name)
        return CMD_UNKNOWN;

    for (int i = 0; COMMAND_ALIASES[i].alias; i++) {

        if (strcmp(COMMAND_ALIASES[i].alias, name) == 0) {

            if (COMMAND_ALIASES[i].access == CMD_LOCKED) {
                event_fire(EVENT_ERROR, "command locked");
                return CMD_UNKNOWN;
            }

            return COMMAND_ALIASES[i].cmd;
        }
    }

    return CMD_UNKNOWN;
}

/*──────────────────────────────────────────────────────────────
 | Internal command name (debug / AI explain)
 *──────────────────────────────────────────────────────────────*/
const char *command_name(CommandType cmd)
{
    switch (cmd) {
        case CMD_LIST:   return "CMD_LIST";
        case CMD_CD:     return "CMD_CD";
        case CMD_MKDIR:  return "CMD_MKDIR";
        case CMD_BOLO:   return "CMD_BOLO";
        case CMD_SAMAY:  return "CMD_SAMAY";

        case CMD_SAKHI:  return "CMD_SAKHI";
        case CMD_NIYU:   return "CMD_NIYU";
        case CMD_MIRA:   return "CMD_MIRA";
        case CMD_KAVYA:  return "CMD_KAVYA";
        case CMD_SHIRI:  return "CMD_SHIRI";

        case CMD_DELETE: return "CMD_DELETE";
        case CMD_COPY:   return "CMD_COPY";
        case CMD_MOVE:   return "CMD_MOVE";

        default:         return "CMD_UNKNOWN";
    }
}

/*──────────────────────────────────────────────────────────────
 | Execute internal command
 | NOTE:
 |  • BOLO printing is handled in interpreter (AST_COMMAND)
 |  • This file only supports commands that need no args
 *──────────────────────────────────────────────────────────────*/
int execute_command(CommandType cmd)
{
    switch (cmd) {

        case CMD_LIST:
            event_fire(EVENT_EXECUTION_SUCCESS, "list executed");
            printf("[LIST] executed (stub)\n");
            return 0;

        case CMD_CD:
            event_fire(EVENT_EXECUTION_SUCCESS, "cd executed");
            printf("[CD] executed (stub)\n");
            return 0;

        case CMD_MKDIR:
            event_fire(EVENT_EXECUTION_SUCCESS, "mkdir executed");
            printf("[MKDIR] executed (stub)\n");
            return 0;

        case CMD_BOLO:
            /* printing is done in interpreter */
            event_fire(EVENT_EXECUTION_SUCCESS, "bolo executed");
            return 0;

        case CMD_SAMAY: {
            /* ✅ print system time in 12-hour format (AM/PM) */
            event_fire(EVENT_EXECUTION_SUCCESS, "samay executed");

            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            if (!t) {
                printf("00:00 AM\n");
                return 0;
            }

            int hour = t->tm_hour;
            int minute = t->tm_min;

            const char *ampm = "AM";
            if (hour >= 12) ampm = "PM";

            hour = hour % 12;
            if (hour == 0) hour = 12;

            printf("%02d:%02d %s\n", hour, minute, ampm);
            return 0;
        }

        case CMD_SAKHI:
        case CMD_NIYU:
        case CMD_MIRA:
        case CMD_KAVYA:
        case CMD_SHIRI:
            /* handled inside interpreter because requires args */
            event_fire(EVENT_ERROR, "AI command requires args");
            return -1;
        default:
            event_fire(EVENT_ERROR, "command not implemented");
            return -1;
    }
}
