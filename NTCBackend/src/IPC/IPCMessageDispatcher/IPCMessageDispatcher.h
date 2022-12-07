#pragma once

namespace NTC
{
    class IPCMessageDispatcher
    {
    public:
        IPCMessageDispatcher(const IPCMessageDispatcher&) = delete;
        IPCMessageDispatcher(const IPCMessageDispatcher&&) = delete;
        IPCMessageDispatcher& operator=(const IPCMessageDispatcher&) = delete;
        IPCMessageDispatcher& operator=(const IPCMessageDispatcher&&) = delete;
        
        virtual ~IPCMessageDispatcher() = default;

        inline static void Run() { Instance.RunImp(); }

    protected:
        IPCMessageDispatcher() = default;

        void RunImp();

        void MessageProc(const std::string& message);

        void LogMessageProc(std::string_view prefix,const  std::string& message);

        static IPCMessageDispatcher Instance;
    };
    
}
