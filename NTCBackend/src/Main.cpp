#include "NTCpch.h"

#include "FrontendProcess/FrontendProcessLauncher/FrontendProcessLauncher.h"
#include "IPC/IPCController/IPCController.h"
#include "IPC/IPCMessageDispatcher/IPCMessageDispatcher.h"
#include "Logger/Logger.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    try
    {
        NTC::Logger::Init();
        NTC::FrontendProcessLauncher::Launch();

        if (NTC::LaunchStatus status = NTC::FrontendProcessLauncher::GetStatus())
            return status;
        
        auto IPCMD = std::async(std::launch::async, &NTC::IPCMessageDispatcher::Run);
        NTC::IPCController::Run();
        
        IPCMD.wait();
        
    }
    catch (std::runtime_error& e)
    {
        NTC_ERROR(e.what());
    }

    NTC_TRACE("App closed");
    return 0;
}