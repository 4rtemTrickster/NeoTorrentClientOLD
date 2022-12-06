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

        inline static void AddMessage(const std::string& message) { Instance.AddMessageImp(message); }

    protected:
        IPCMessageDispatcher() { MessageQueue = CreateRef<MessageQueue_t>(); }

        void RunImp();

        void AddMessageImp(const std::string& message);

        void MessageProc(const std::string& message);

        void LogMessageProc(std::string_view prefix,const  std::string& message);

        static IPCMessageDispatcher Instance;

        Ref<MessageQueue_t> MessageQueue;
    };
    
}
