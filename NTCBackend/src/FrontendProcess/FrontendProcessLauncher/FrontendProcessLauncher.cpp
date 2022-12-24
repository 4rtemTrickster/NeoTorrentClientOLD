#include "NTCpch.h"
#include "FrontendProcessLauncher.h"

namespace NTC
{
    void FrontendProcessLauncher::Launch()
    {
        if (!FrontendProcessInstance)
        {
            NTC_TRACE("Frontend starting up");

            FrontendProcessInstance = ShellExecute(NULL, NULL, exe, NULL, NULL, SW_SHOW);

            if ((INT_PTR)FrontendProcessInstance > 32)
                NTC_TRACE("Frontend started up successful");
            else
            {
                Error = GetLastError();
                NTC_ERROR("Frontend process creation failed! Error code: {0}", Error);
            }
        }
        else
            NTC_INFO("Frontend started up already but Init is called another time!");
    }
}