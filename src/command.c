#include <stdio.h>
#include <string.h>

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

    /* LOCKED / RESERVED */
    { "shyam",     CMD_DELETE, CMD_LOCKED },
    { "keshav",    CMD_COPY,  CMD_LOCKED },

    { NULL, CMD_UNKNOWN, CMD_LOCKED }
};


/*──────────────────────────────────────────────────────────────
 | Resolve user-facing name → internal command
 *──────────────────────────────────────────────────────────────*/
CommandType resolve_command(const char *name) {

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
const char *command_name(CommandType cmd) {

    switch (cmd) {
        case CMD_LIST:   return "CMD_LIST";
        case CMD_CD:     return "CMD_CD";
        case CMD_MKDIR:  return "CMD_MKDIR";
        case CMD_DELETE: return "CMD_DELETE";
        case CMD_COPY:   return "CMD_COPY";
        case CMD_MOVE:   return "CMD_MOVE";
        default:         return "CMD_UNKNOWN";
    }
}


/*──────────────────────────────────────────────────────────────
 | Execute internal command (STUB)
 *──────────────────────────────────────────────────────────────*/
int execute_command(CommandType cmd) {

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

        default:
            event_fire(EVENT_ERROR, "command not implemented");
            return -1;
    }
}
