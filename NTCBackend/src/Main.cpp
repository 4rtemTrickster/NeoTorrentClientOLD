#include "NTCpch.h"

#include "FrontendProcess/FrontendProcessLauncher/FrontendProcessLauncher.h"
#include "IPC/IPCController/IPCController.h"
#include "IPC/IPCMessageDispatcher/IPCMessageDispatcher.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    try
    {
        NTC_PROFILE_BEGIN_SESSION("Launch", "../Profiling/Startup.json");
        
        NTC::Logger::Init();
        NTC::FrontendProcessLauncher::Launch();

        if (NTC::LaunchStatus status = NTC::FrontendProcessLauncher::GetStatus())
            return status;

        NTC_PROFILE_END_SESSION();
        

        NTC_PROFILE_BEGIN_SESSION("Work", "../Profiling/Working.json");

        auto IPCC = std::async(std::launch::async, &NTC::IPCController::Run);
        NTC::IPCMessageDispatcher::Run();

        IPCC.wait();

        NTC_PROFILE_END_SESSION();
    }
    catch (std::exception& e)
    {
        NTC_ERROR(e.what());
    }

    NTC_TRACE("App closed");
    return 0;
}