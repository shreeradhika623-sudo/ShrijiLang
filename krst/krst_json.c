#include <stdio.h>

#include "../include/krst_json.h"

void krst_response_to_json(
    KRSTResponse *res,
    char *buffer,
    size_t size
)
{
    if (!res || !buffer)
        return;

    snprintf(
        buffer,
        size,

        "{"
        "\"success\":%s,"
        "\"output\":\"%s\","
        "\"explain\":\"%s\","
        "\"error\":\"%s\""
        "}",

        (res->type == KRST_RESPONSE_ERROR)
            ? "false"
            : "true",

        res->result_text,
        res->explain_text,
        res->message
    );
}
