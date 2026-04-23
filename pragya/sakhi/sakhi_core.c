#include "sakhi_core.h"
#include <stdio.h>

L3Response sakhi_speak(const L3Request *request,
                       const NiyuResult *result)
{
    (void)result;

    L3Response res;

    if (!request || !request->raw_input)
    {
        res.text = "Input samajh nahi aaya.";
        res.success = 0;
        return res;
    }

    printf("[PRAGYA] Main sun rahi hoon. Thoda clear likhoge to aur achha ho jayega 🙂\n");

    res.text = request->raw_input;
    res.success = 1;

    return res;
}
