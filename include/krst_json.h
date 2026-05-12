#ifndef KRST_JSON_H
#define KRST_JSON_H

#include <stddef.h>

#include "../krst/krst_types.h"

void krst_response_to_json(
    KRSTResponse *res,
    char *buffer,
    size_t size
);

#endif
