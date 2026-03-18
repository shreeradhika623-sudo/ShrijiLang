#ifndef SHRIJI_ERROR_INTELLIGENCE_H
#define SHRIJI_ERROR_INTELLIGENCE_H

#include "error.h"
#include "../pragya/niyu/niyu_contract.h"
#include "../include/pragya_avastha.h"

/* error clarity levels */

#define ERR_CONFUSED 0
#define ERR_PARTIAL  1
#define ERR_CLEAR    2


/* understanding engine */

int shriji_understand_error(
    ShrijiErrorCode code,
    const NiyuResult *niyu
);


/* pointer extractor */

void shriji_extract_pointer(
    const ShrijiErrorInfo *err,
    int *pos,
    int *len
);


/* main intelligence entry */

void shriji_error_intelligence(
    PragyaAvastha *avastha,
    const ShrijiErrorInfo *err,
    const NiyuResult *niyu
);

#endif
