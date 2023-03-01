#include "NTCpch.h"
#include "Downloader.h"

#include "Torrent/TorrentFile/SingleFileTorrent/SingleFileTorrent.h"

#include <cpr/cpr.h>

namespace NTC
{
    void Downloader::DownloadFile(Ref<SingleFileTorrent> file)
    {
        cpr::Response response = cpr::Get(cpr::Url{file->GetAnnounce()},
            cpr::Parameters
            {
                {"info_hash", file->GetInfoHash()},
                {"peer_id", ClientId},
                {"uploaded", "0"},
                {"downloaded", "0"},
                {"left", std::to_string(file->GetLength())},
                {"port", "8080"},
                {"compact", "1"}
            }, cpr::Timeout{15000});
        
        if(response.status_code == 200)
        {
            NTC_INFO("Recived from tracker: " + response.text);
        }
    }
}
