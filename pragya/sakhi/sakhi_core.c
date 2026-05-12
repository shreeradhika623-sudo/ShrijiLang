#include "sakhi_core.h"
#include <stdio.h>

L3Response sakhi_speak(const L3Request *request,
                       const NiyuResult *result)
{
    (void)result;

    L3Response res;

    // Input validation
    if (!request || !request->raw_input)
    {
        res.text = "Invalid input";
        res.success = 0;
        return res;
    }

    // Minimal clean log (optional, can remove later)
    printf("[PRAGYA] Processing input...\n");

    // Pass-through response
    res.text = request->raw_input;
    res.success = 1;

    return res;
}
