#ifndef VALUE_H
#define VALUE_H

#include <string.h>
#include <stdlib.h>

#include "ast.h"

/*──────────────────────────────────────────────
  VALUE TYPES
──────────────────────────────────────────────*/
typedef enum {
    VAL_NULL = 0,
    VAL_NUMBER,
    VAL_STRING,
    VAL_BOOL,
    VAL_LIST,
    VAL_FUNCTION,
    VAL_DICT
} ValueType;

typedef struct Value {
    ValueType type;

    double number;
    char *string;
    int boolean;
    /* ✅ list value */
    struct Value *items;
    int count;

    /* ✅ function value */
    ASTNode *function;

    /* ✅ dict value */
    struct Value *dict_keys;
    struct Value *dict_values;
    int dict_count;

} Value;

/*──────────────────────────────────────────────
  CONSTRUCTORS
──────────────────────────────────────────────*/
static inline Value value_null(void)
{
    Value v;
    v.type = VAL_NULL;

    v.number = 0;
    v.string = NULL;

    v.items = NULL;
    v.count = 0;

    v.function = NULL;

    v.dict_keys = NULL;
    v.dict_values = NULL;
    v.dict_count = 0;

    return v;
}

static inline Value value_number(double n)
{
    Value v = value_null();
    v.type = VAL_NUMBER;
    v.number = n;
    return v;
}

static inline Value value_string(const char *s)
{
    Value v = value_null();
    v.type = VAL_STRING;

    if (!s) {
        v.string = NULL;
        return v;
    }

    v.string = (char *)malloc(strlen(s) + 1);
    if (!v.string) return value_null();

    strcpy(v.string, s);
    return v;
}

/* BOOL SUPPORT */
static inline Value value_bool(int b)
{
    Value v = value_null();
    v.type = VAL_BOOL;
    v.boolean = b ? 1 : 0;
    return v;
}

static inline int value_is_truthy(Value v)
{
    switch (v.type) {
        case VAL_BOOL:   return v.boolean;
        case VAL_NUMBER: return v.number != 0;
        case VAL_STRING: return v.string && v.string[0] != '\0';
        case VAL_NULL:   return 0;
        default:         return 1;  // list, dict, function → truthy
    }
}


/* ✅ list constructor */
static inline Value value_list(Value *items, int count)
{
    Value v = value_null();
    v.type = VAL_LIST;
    v.items = items;
    v.count = count;
    return v;
}

/* ✅ function constructor */
static inline Value value_function(ASTNode *fn)
{
    Value v = value_null();
    v.type = VAL_FUNCTION;
    v.function = fn;
    return v;
}

/* ✅ dict constructor */
static inline Value value_dict(Value *keys, Value *values, int count)
{
    Value v = value_null();
    v.type = VAL_DICT;

    v.dict_keys = keys;
    v.dict_values = values;
    v.dict_count = count;

    return v;
}

/*──────────────────────────────────────────────
  COPY
──────────────────────────────────────────────*/
static inline Value value_copy(Value src)
{
    if (src.type == VAL_STRING) {
        return value_string(src.string ? src.string : "");
    }

    if (src.type == VAL_NUMBER) {
        return value_number(src.number);
    }

    if (src.type == VAL_BOOL) {
         return value_bool(src.boolean);
    }


    if (src.type == VAL_LIST) {

        if (src.count <= 0 || !src.items) {
            return value_list(NULL, 0);
        }

        Value *items = (Value *)malloc(sizeof(Value) * src.count);
        if (!items) return value_null();

        for (int i = 0; i < src.count; i++) {
            items[i] = value_copy(src.items[i]);
        }

        return value_list(items, src.count);
    }

    if (src.type == VAL_FUNCTION) {
        /* function node pointer copy (AST is owned elsewhere) */
        return value_function(src.function);
    }

    if (src.type == VAL_DICT) {

        if (src.dict_count <= 0 || !src.dict_keys || !src.dict_values) {
            return value_dict(NULL, NULL, 0);
        }

        Value *keys = (Value *)malloc(sizeof(Value) * src.dict_count);
        Value *values = (Value *)malloc(sizeof(Value) * src.dict_count);

        if (!keys || !values) {
            if (keys) free(keys);
            if (values) free(values);
            return value_null();
        }

        for (int i = 0; i < src.dict_count; i++) {
            keys[i] = value_copy(src.dict_keys[i]);
            values[i] = value_copy(src.dict_values[i]);
        }

        return value_dict(keys, values, src.dict_count);
    }

    return value_null();
}

/*──────────────────────────────────────────────
  FREE
──────────────────────────────────────────────*/
static inline void value_free(Value *v)
{
    if (!v) return;

    if (v->type == VAL_STRING && v->string) {
        free(v->string);
        v->string = NULL;
    }

    if (v->type == VAL_LIST && v->items) {
        for (int i = 0; i < v->count; i++) {
            value_free(&v->items[i]);
        }
        free(v->items);
        v->items = NULL;
        v->count = 0;
    }

    if (v->type == VAL_DICT) {
        if (v->dict_keys) {
            for (int i = 0; i < v->dict_count; i++) {
                value_free(&v->dict_keys[i]);
            }
            free(v->dict_keys);
            v->dict_keys = NULL;
        }

        if (v->dict_values) {
            for (int i = 0; i < v->dict_count; i++) {
                value_free(&v->dict_values[i]);
            }
            free(v->dict_values);
            v->dict_values = NULL;
        }

        v->dict_count = 0;
    }

    /* function is not freed here */
    v->function = NULL;

    /* reset */
    v->type = VAL_NULL;
    v->number = 0.0;
    v->string = NULL;

    v->items = NULL;
    v->count = 0;

    v->dict_keys = NULL;
    v->dict_values = NULL;
    v->dict_count = 0;
}


#endif
