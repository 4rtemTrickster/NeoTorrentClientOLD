#include "NTCpch.h"
#include "IPCMessageDispatcher.h"

#include <filesystem>

#include "FileReader/FileReader.h"
#include "IPC/MessageQueue/MessageQueue.h"
#include "Torrent/Bencode/BencodeDecoder/BencodeDecoder.h"

namespace NTC
{
    IPCMessageDispatcher IPCMessageDispatcher::Instance;

    void IPCMessageDispatcher::RunImp()
    {
        try
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
        catch (std::exception& exception)
        {
            NTC_CRITICAL(exception.what());
        }
    }

    void IPCMessageDispatcher::MessageProc(const std::string& message)
    {
        std::string_view Prefix(message.c_str(), message.find_first_of(' '));

        if (Prefix.starts_with("LOG:")) LogMessageProc(Prefix, message);
        if (Prefix.starts_with("READ_FILE:"))
        {
            auto readed = FileReader::ReadFile(
                std::filesystem::path(std::string(message.c_str() + Prefix.size() + 1)));
            
            auto dec = BencodeDecoder::Decode(*readed);
        }
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
