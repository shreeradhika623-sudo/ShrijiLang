#include "mira_core.h"

/*
 * Minimal Mira implementation.
 * Teaching-only response.
 * Future versions will expand lesson structure.
 */

L3Response mira_teach(const L3Request *request)
{
    (void)request;

    L3Response response;
    response.text = "Teaching mode active. Mira is ready.";
    response.success = 1;
    return response;
}
