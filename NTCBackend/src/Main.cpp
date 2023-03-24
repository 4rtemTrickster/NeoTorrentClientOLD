#include "NTCpch.h"

#include "AppSettings/AppSettings.h"
#include "FrontendProcess/FrontendProcessLauncher/FrontendProcessLauncher.h"
#include "IPC/IPCController/IPCController.h"
#include "IPC/IPCMessageDispatcher/IPCMessageDispatcher.h"
#include "Torrent/TorrentPeer/TorrentPeer.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    std::future<std::invoke_result_t<void(*)()>> IPCC;
    std::future<void> NetP;
    try
    {
        NTC_PROFILE_BEGIN_SESSION("Launch", "../Profiling/Startup.json");

        NTC::Logger::Init();
        NTC::FrontendProcessLauncher::Launch();

        if (NTC::LaunchStatus status = NTC::FrontendProcessLauncher::GetStatus())
            return status;

        NTC::ThreadPool::Init();

        //TODO: Change to settings file
        NTC::AppSettings::ClientId = "-NT0001-";
        
        std::random_device rd; // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution distrib(1, 9);
        for (int i = 0; i < 12; i++)
            NTC::AppSettings::ClientId += std::to_string(distrib(gen));

        NTC::AppSettings::Port = 8080;

        NetP = NTC::ThreadPool::submit(&NTC::TorrentPeer::StartLoop);

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
