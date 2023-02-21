#include "NTCpch.h"

#include "FrontendProcess/FrontendProcessLauncher/FrontendProcessLauncher.h"
#include "IPC/IPCController/IPCController.h"
#include "IPC/IPCMessageDispatcher/IPCMessageDispatcher.h"



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    std::future<std::invoke_result_t<void(*)()>> IPCC;
    try
    {
        NTC_PROFILE_BEGIN_SESSION("Launch", "../Profiling/Startup.json");
        
        NTC::Logger::Init();
        NTC::FrontendProcessLauncher::Launch();

        if (NTC::LaunchStatus status = NTC::FrontendProcessLauncher::GetStatus())
            return status;

        NTC::ThreadPool::Init();

        NTC_PROFILE_END_SESSION();
        

        NTC_PROFILE_BEGIN_SESSION("Work", "../Profiling/Working.json");

        
        IPCC = NTC::ThreadPool::submit(&NTC::IPCController::Run);
        
        NTC::IPCMessageDispatcher::Run();

        IPCC.wait();

        NTC_PROFILE_END_SESSION();
    }
    catch (std::exception& e)
    {
        IPCC.wait();
        NTC_ERROR(e.what());
    }

    NTC_TRACE("App closed");
    return 0;
}
