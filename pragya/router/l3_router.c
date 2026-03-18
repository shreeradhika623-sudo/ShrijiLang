#include "l3_router.h"

#include "../niyu/niyu_contract.h"
#include "../sakhi/sakhi_contract.h"
#include "../mira/mira_contract.h"

#include "../../include/pragya_avastha.h"

#include <stdio.h>

/*
    Pragya Router

    Role:
    KRST se aayi hui avastha ko analyze karna
    aur decide karna ki Niyu / Mira / Sakhi kaise use hongi.
*/

void pragya_route(PragyaAvastha *avastha)
{
    if (!avastha)
        return;

    /* ========================= */
    /* SILENT MODE               */
    /* ========================= */

    if (avastha->teach_level == 0)
    {
        return;
    }

    /* ========================= */
    /* NIYU ANALYSIS             */
    /* ========================= */

    NiyuResult *result = niyu_think_avastha(avastha);

    if (!result)
        return;

    /* ========================= */
    /* MIRA TEACHING             */
    /* ========================= */

    if (avastha->teach_level > 1)
    {
        mira_teach_avastha(avastha, result);
    }

    /* ========================= */
    /* SAKHI RESPONSE            */
    /* ========================= */

    if (avastha->error_code != 0)
    {
        L3Request request;

        request.raw_input = avastha->raw_text;
        request.intent = 0;

        sakhi_speak(&request, result);
    }

    /* ========================= */
    /* MEMORY CLEANUP            */
    /* ========================= */

    niyu_free_result(result);
}
