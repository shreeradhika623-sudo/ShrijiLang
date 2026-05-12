#ifndef GYAAN_ENGINE_H
#define GYAAN_ENGINE_H

#include "../../include/error.h"

/* ==============================
   GYAAN DATA ACCESS (CLEAN API)
   ============================== */

/* Core explanation */
const char* gyaan_get(const ShrijiErrorInfo *err);

/* Optional layers */
const char* gyaan_get_hint(const ShrijiErrorInfo *err);
const char* gyaan_get_rule(const ShrijiErrorInfo *err);

#endif
