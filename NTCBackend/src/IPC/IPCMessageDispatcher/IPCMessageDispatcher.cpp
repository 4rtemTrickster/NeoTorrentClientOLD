#include "NTCpch.h"
#include "IPCMessageDispatcher.h"

#include "IPC/MessageQueue/MessageQueue.h"

namespace NTC
{
    IPCMessageDispatcher IPCMessageDispatcher::Instance;

    void IPCMessageDispatcher::RunImp()
    {
        std::string Message;
        while (true)
        {
            MessageQueue::PopMessage(Message);
            LOG_MESSAGE("Poped message: " + Message);
            if (Message == "App closes") break;

            MessageProc(Message);
        }

        NTC_TRACE("Message dispatching is ended");
    }

    void IPCMessageDispatcher::MessageProc(const std::string& message)
    {
        std::string_view Prefix(message.c_str(), message.find_first_of(' '));

        if (Prefix.starts_with("LOG:")) LogMessageProc(Prefix, message);
    }

    void IPCMessageDispatcher::LogMessageProc(std::string_view prefix, const std::string& message)
    {
        const std::string_view LogType(prefix.substr(4));

        if (LogType == "TRACE")
        {
            FRONTEND_TRACE(std::string_view(message.c_str() + 10));
            return;
        }
        if (LogType == "INFO")
        {
            FRONTEND_INFO(std::string_view(message.c_str() + 9));
            return;
        }
        if (LogType == "WARN")
        {
            FRONTEND_WARN(std::string_view(message.c_str() + 9));
            return;
        }
        if (LogType == "ERROR")
        {
            FRONTEND_ERROR(std::string_view(message.c_str() + 10));
            return;
        }
        if (LogType == "CRITICAL")
        {
            FRONTEND_CRITICAL(std::string_view(message.c_str() + 13));
            return;
        }

        NTC_ERROR("Received unknown log level!");
    }
}
