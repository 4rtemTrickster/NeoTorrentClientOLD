#include "NTCpch.h"
#include "Downloader.h"

#include "AppSettings/AppSettings.h"
#include "Net/IPeer/IPeer.h"
#include "PeerRetriever/PeerRetriever.h"

namespace NTC
{
    enum class id : uint8_t
    {
        LP_KeepAlive = UCHAR_MAX - 1,
        LP_Choke = 0,
        LP_Unchoke = 1,
        LP_Interested = 2,
        LP_NotInterested = 3,
        LP_Have = 4,
        LP_Bitfield = 5,
        LP_Request = 6,
        LP_Piece = 7,
        LP_Cancel = 8,
        LP_Port = 9
    };

    class CustomNetCommunicator : public IPeer<id>
    {
    public:
        CustomNetCommunicator(uint16_t port, std::string&& handshakeMsg)
            : IPeer<id>(port, std::forward<std::string>(handshakeMsg))
        {
        }

    protected:
        virtual bool OnConnection(Ref<Connection<id>> con) override
        {
            NTC_TRACE("OnConnection");
            return true;
        }

        virtual void OnDisconnection(Ref<Connection<id>> con) override
        {
            NTC_TRACE("OnDisconnection");
        }

        virtual void OnMessage(Ref<Connection<id>> src, Message<id> msg) override
        {
            switch (msg.Header.Id)
            {
            case id::LP_KeepAlive:
                NTC_TRACE("[{0}] Get KeepAlive", src->GetEndpoint());
                break;
            case id::LP_Choke:
                NTC_TRACE("[{0}] Get Choke", src->GetEndpoint());
                break;
            case id::LP_Unchoke:
                NTC_TRACE("[{0}] Get Unchoke", src->GetEndpoint());
                break;
            case id::LP_Interested:
                NTC_TRACE("[{0}] Get Interested", src->GetEndpoint());
                break;
            case id::LP_NotInterested:
                NTC_TRACE("[{0}] Get NotInterested", src->GetEndpoint());
                break;
            case id::LP_Have:
                NTC_TRACE("[{0}] Get Have", src->GetEndpoint());
                break;
            case id::LP_Bitfield:
                NTC_TRACE("[{0}] Get Bitfield", src->GetEndpoint());
                break;
            case id::LP_Request:
                NTC_TRACE("[{0}] Get Request", src->GetEndpoint());
                break;
            case id::LP_Piece:
                NTC_TRACE("[{0}] Get Piece", src->GetEndpoint());
                break;
            case id::LP_Cancel:
                NTC_TRACE("[{0}] Get Cancel", src->GetEndpoint());
                break;
            case id::LP_Port:
                NTC_TRACE("[{0}] Get Port", src->GetEndpoint());
                break;

            default:
                NTC_ERROR("[{0}] Incorrect message id: {1}", src->GetEndpoint(), std::to_string(static_cast<int>(msg.Header.Id)));
            }
        }

        virtual void OnValidated(Ref<Connection<id>> con) override
        {
            NTC_TRACE("OnValidated");
        }
    };

    void Downloader::DownloadFile(Ref<ITorrentFile> file)
    {
        Ref<std::vector<Ref<Peer>>> Peers = PeerRetriever::GetPeersList(file);

        if (!Peers) return;

        const std::string protocol = "BitTorrent protocol";
        std::stringstream buffer;
        buffer << (char)protocol.length();
        buffer << protocol;
        std::string reserved;
        for (int i = 0; i < 8; i++)
            reserved.push_back('\0');
        buffer << reserved;
        buffer << file->GetInfoHash();
        buffer << AppSettings::ClientId;
        assert(buffer.str().length() == protocol.length() + 49);

        CustomNetCommunicator communicator(8080, std::string(buffer.str()));
        communicator.Start();


        for (auto& peer : *Peers)
        {
            communicator.Connect(peer->IP, static_cast<uint16_t>(peer->Port));
        }

        while (true)
        {
            communicator.Update();
        }
    }
}
