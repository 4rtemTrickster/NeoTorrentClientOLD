#include "NTCpch.h"
#include "IPCMessageDispatcher.h"

#include <filesystem>

#include "FileReader/FileReader.h"
#include "IPC/MessageQueue/MessageQueue.h"
#include "Torrent/Bencode/BencodeDecoder/BencodeDecoder.h"
#include "Torrent/Bencode/BencodeTypes/BencodeDictionary/BencodeDictionary.h"
#include "Torrent/TorrentFileFactory/TorrentFileFactory.h"

namespace NTC
{
    IPCMessageDispatcher IPCMessageDispatcher::Instance;

    void IPCMessageDispatcher::RunImp()
    {
        NTC_PROFILE_FUNCTION();
        std::string Message;
        while (true)
        {
            try
            {
                MessageQueue::PopMessage(Message);
                LOG_MESSAGE("Poped message: " + Message);
                if (Message == "App closes") break;

                MessageProc(Message);
            }
            catch (std::exception& exception)
            {
                NTC_CRITICAL(exception.what());
            }
        }

        NTC_TRACE("Message dispatching is ended");
    }

    void IPCMessageDispatcher::MessageProc(const std::string& message)
    {
        NTC_PROFILE_FUNCTION();
        std::string_view Prefix(message.c_str(), message.find_first_of(' '));

        if (Prefix.starts_with("LOG:")) LogMessageProc(Prefix, message);
        if (Prefix.starts_with("READ_FILE:"))
        {
            auto readed = FileReader::ReadFile(
                std::filesystem::path(std::string(message.c_str() + Prefix.size() + 1)));

            Ref<BencodeDictionary> dic = std::dynamic_pointer_cast<BencodeDictionary>(BencodeDecoder::Decode(*readed));

            auto f = TorrentFileFactory::CreateTorrentFile(dic);

            if(f != nullptr)
                NTC_TRACE("Torrent file created!");
            else
                NTC_TRACE("Error during torrent file creation!");
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
