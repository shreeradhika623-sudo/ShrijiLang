#ifndef COMMAND_H
#define COMMAND_H

/*──────────────────────────────────────────────────────────────
 | INTERNAL COMMAND IDENTIFIERS (TRUTH)
 *──────────────────────────────────────────────────────────────*/
typedef enum {

    CMD_UNKNOWN = 0,

    /* Filesystem */
    CMD_LIST,        /* ls */
    CMD_CD,          /* cd */
    CMD_MKDIR,       /* mkdir */

    /* Output / I/O */
    CMD_BOLO,        /* bolo (print) */
    CMD_SAMAY,

/* AI Modules */
    CMD_SAKHI,
    CMD_NIYU,
    CMD_MIRA,
    CMD_KAVYA,
    CMD_SHIRI,

    /* Future expansion */
    CMD_DELETE,
    CMD_COPY,
    CMD_MOVE



} CommandType;


/*──────────────────────────────────────────────────────────────
 | COMMAND ACCESS POLICY
 *──────────────────────────────────────────────────────────────*/
typedef enum {
    CMD_PUBLIC = 1,     /* visible to everyone */
    CMD_LOCKED = 0      /* reserved / internal */
} CommandAccess;


/*──────────────────────────────────────────────────────────────
 | COMMAND ALIAS (1008 NAMES LIVE HERE)
 *──────────────────────────────────────────────────────────────*/
typedef struct {
    const char     *alias;     /* user-facing name */
    CommandType     cmd;       /* internal truth */
    CommandAccess   access;    /* public / locked */
} CommandAlias;


/*──────────────────────────────────────────────────────────────
 | API
 *──────────────────────────────────────────────────────────────*/
CommandType resolve_command(const char *name);
const char *command_name(CommandType cmd);
int execute_command(CommandType cmd);

#endif /* COMMAND_H */
