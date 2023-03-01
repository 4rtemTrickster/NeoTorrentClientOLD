#pragma once
#include "Torrent/TorrentFile/ITorrentFile.h"
#include "Torrent/TorrentFile/SingleFileTorrent/SingleFileTorrent.h"

namespace NTC
{
    class Downloader
    {
    public:
        static void DownloadFile(Ref<SingleFileTorrent> file);
    protected:
        inline static const std::string ClientId = "-PC0001-706887310628";
    };
}
