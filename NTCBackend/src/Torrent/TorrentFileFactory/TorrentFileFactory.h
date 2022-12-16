#pragma once
#include "Torrent/Bencode/BencodeTypes/BencodeDictionary/BencodeDictionary.h"
#include "Torrent/TorrentFile/ITorrentFile.h"

namespace NTC
{
    class TorrentFileFactory
    {
    public:
        TorrentFileFactory() = delete;
        TorrentFileFactory(const TorrentFileFactory&) = delete;
        TorrentFileFactory(TorrentFileFactory&&) = delete;

        [[nodiscard]]
        static Ref<ITorrentFile> CreateTorrentFile(const Ref<BencodeDictionary>& dictionary);    
    };
}
