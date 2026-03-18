#ifndef GYAAN_ENGINE_H
#define GYAAN_ENGINE_H

#include "../../include/error.h"

typedef struct {

    ShrijiErrorCode error;

    const char *module;
    const char *file;
    const char *function;

    const char *rule;
    const char *explain;
    const char *hint;

} GyaanEntry;

const GyaanEntry* gyaan_lookup(ShrijiErrorCode code);

void gyaan_print(const ShrijiErrorInfo *err);

#endif
