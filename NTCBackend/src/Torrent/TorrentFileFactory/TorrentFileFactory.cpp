#include "NTCpch.h"
#include "TorrentFileFactory.h"

#include "Torrent/TorrentFile/MultipleFileTorrent/MultipleFileTorrent.h"
#include "Torrent/TorrentFile/SingleFileTorrent/SingleFileTorrent.h"

namespace NTC
{
#define CastToDictionary std::dynamic_pointer_cast<BencodeDictionary>
#define CastToList std::dynamic_pointer_cast<BencodeList>
#define CastToString std::dynamic_pointer_cast<BencodeString>
#define CastToInt std::dynamic_pointer_cast<BencodeInt>

    Ref<ITorrentFile> TorrentFileFactory::CreateTorrentFile(const Ref<BencodeDictionary>& dictionary)
    {
        Ref<ITorrentFile> toReturn;
        auto GlobalDic = dictionary->GetValue();
        auto InfoDic = CastToDictionary(GlobalDic.at("info"))->GetValue();

        if(InfoDic.contains("files"))
        {
            toReturn = CreateRef<MultipleFileTorrent>();
        }
        else
        {
            toReturn = CreateRef<SingleFileTorrent>();
        }

        return toReturn;
    }
}
