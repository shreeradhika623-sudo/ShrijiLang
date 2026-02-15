#include "niyu_core.h"
#include <stdlib.h>

/*
 * Minimal internal result.
 * Future me is structure ko replace kiya ja sakta hai
 * bina kisi external file ko touch kiye.
 */
struct NiyuResult {
    int placeholder;
};

NiyuResult *niyu_think(const L3Request *request)
{
    (void)request;

    NiyuResult *res = (NiyuResult *)malloc(sizeof(NiyuResult));
    if (!res) {
        return NULL;
    }

    /* Minimal placeholder value */
    res->placeholder = 0;
    return res;
}

void niyu_free_result(NiyuResult *result)
{
    if (result) {
        free(result);
    }
}
