// SHRIJI_RADHR RADHR

#ifndef SHRIJI_TOKEN_LIST_H
#define SHRIJI_TOKEN_LIST_H

/*
──────────────────────────────────────────────
 ShrijiLang Token System V2
 Dynamic Token Storage (Compiler Grade)
──────────────────────────────────────────────
*/

#include "token.h"

/*──────────────────────────────────────────────
  TOKEN LIST STRUCTURE
──────────────────────────────────────────────*/

typedef struct
{
    Token *data;      // token array
    int count;        // number of tokens used
    int capacity;     // allocated memory

} TokenList;


/*──────────────────────────────────────────────
  TOKEN LIST API
──────────────────────────────────────────────*/

/* create new list */
TokenList token_list_create(void);

/* add token */
void token_list_push(TokenList *list, Token tok);

/* get token */
Token token_list_get(TokenList *list, int index);

/* number of tokens */
int token_list_size(TokenList *list);

/* free memory */
void token_list_free(TokenList *list);


#endif
