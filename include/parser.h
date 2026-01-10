#ifndef PARSER_H
#define PARSER_H

//──────────────────────────────────────────────────────────────
//  SHRIJILANG PARSER — HEADER (UNIVERSE-CLASS VERSION)
//
//  This module converts raw source code strings into AST Nodes.
//  
//  Core features supported in V1:
//    ✔ mavi assignment       → mavi x = 10 + 20
//    ✔ arithmetic expressions
//    ✔ sakhi "msg"
//    ✔ niyu "msg"
//    ✔ shiri "msg"
//    ✔ mira "msg"
//    ✔ kavya "msg"
//    ✔ sri_<command> "msg"
//    ✔ GAURI Super-List Engine (V1.5)
//
//  Upcoming in V2:
//    ✔ multi-statement blocks { }
//    ✔ loops
//    ✔ conditions
//──────────────────────────────────────────────────────────────

#include "ast.h"

//──────────────────────────────────────────────────────────────
//  PARSE ENTRY POINT
//──────────────────────────────────────────────────────────────
//
//  parse_program()
//      Converts a full input string into a root AST node.
//      Returns NULL if syntax error.
//
ASTNode *parse_program(const char *source);


//──────────────────────────────────────────────────────────────
//  GAURI DATA-STRUCTURE PARSER DECLARATIONS (NEW)
//──────────────────────────────────────────────────────────────
//
//  Gauri commands allow creation + mutation of "living lists"
//  Example:
//      gauri_list x = "[1,2,3]"
//      gauri_push x 50
//      gauri_pop x
//      gauri_get x 2
//      gauri_set x 2 99
//      gauri_sum x
//      gauri_sort x
//      gauri_reverse x
//
ASTNode* parse_gauri_list();
ASTNode* parse_gauri_push();
ASTNode* parse_gauri_pop();
ASTNode* parse_gauri_get();
ASTNode* parse_gauri_set();
ASTNode* parse_gauri_sum();
ASTNode* parse_gauri_sort();
ASTNode* parse_gauri_reverse();

#endif
