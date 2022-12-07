#pragma once

namespace NTC
{
    class FrontendProcessLauncher
    {
    public:
        static void Launch();
        inline static LaunchStatus GetStatus() { return Error; }
    protected:
        
        inline static HINSTANCE FrontendProcessInstance = nullptr;
        inline static constexpr LPCWSTR exe = L"..\\NTCFrontend\\NTCFrontend.exe";
        inline static DWORD Error = 0;
    
    };
}
