#include "NTCpch.h"
#include "TorrentFileFactory.h"

#include "Torrent/TorrentFile/MultipleFileTorrent/MultipleFileTorrent.h"
#include "Torrent/TorrentFile/SingleFileTorrent/SingleFileTorrent.h"

#include "Torrent/Bencode/BencodeTypes/BencodeInt/BencodeInt.h"
#include "Torrent/Bencode/BencodeTypes/BencodeList/BencodeList.h"

template <class T>
using optional = boost::optional<T>;

namespace NTC
{
    Ref<ITorrentFile> TorrentFileFactory::CreateTorrentFile(Ref<BencodeDictionary>& dictionary)
    {
        Ref<BencodeDictionary> InfoDic = DynamicCast<BencodeDictionary>(dictionary->at("info"));

        optional<std::string&> announce = TryGetValue<BencodeString>(dictionary, "announce");
        optional<int64_t&> pieceLength = TryGetValue<BencodeInt>(dictionary, "pieceLength");
        optional<std::string&> pieces = TryGetValue<BencodeString>(dictionary, "pieces");
        optional<std::string&> name = TryGetValue<BencodeString>(InfoDic, "name");
        Ref<std::vector<Hash_t>> piecesHashes;
        if (pieces.has_value()) piecesHashes = SeparatePiecesStr(pieces.value());


        Ref<ITorrentFile> toReturn;

        if (InfoDic->contains("files"))
        {
            Ref<BencodeList> FilesList = DynamicCast<BencodeList>(InfoDic->at("files"));

            std::list<MultipleFileTorrent::file> files;
            for (auto& element : *FilesList)
            {
                Ref<BencodeDictionary> dic = DynamicCast<BencodeDictionary>(element);
                optional<int64_t&> length = TryGetValue<BencodeInt>(dic, "length");
                
                Ref<BencodeList> pathList = DynamicCast<BencodeList>(dic->at("path"));
                std::list<std::string> path;
                for (auto& pathPart : *pathList)
                    path.push_back(DynamicCast<BencodeString>(pathPart)->GetValue()); //TODO: Safe!!!!

                if(length.has_value())
                    files.emplace_back(*length, std::move(path));
                else
                {
                    NTC_ERROR("Cannot create a torrent file representation 'cause one of the required fields is not filled in");
                    return nullptr;
                }
            }

            toReturn = CreateRef<MultipleFileTorrent>(std::move(announce), pieceLength, std::move(*piecesHashes),
                                                      std::move(name), std::move(files));
        }
        else
        {
            optional<int64_t&> length = TryGetValue<BencodeInt>(InfoDic, "length");

            if (announce.has_value() &&
                pieceLength.has_value() &&
                name.has_value() &&
                pieces.has_value()
                && length.has_value()
            )
                toReturn = CreateRef<SingleFileTorrent>(std::move(announce), pieceLength, std::move(*piecesHashes),
                                                        std::move(name), length);
            else
            {
                NTC_ERROR("Cannot create a torrent file representation 'cause one of the required fields is not filled in");
                return nullptr;
            }
        }

        return toReturn;
    }

    Ref<std::vector<Hash_t>> TorrentFileFactory::SeparatePiecesStr(std::string& pieces)
    {
        Ref<std::vector<Hash_t>> pieceHashes = CreateRef<std::vector<Hash_t>>();
        pieceHashes->reserve(pieces.size() / 20);

        for (std::size_t i = 0; i < pieces.size(); i += 20)
        {
            Hash_t hash;

            for (std::size_t j = 0; j < 20; ++j)
                hash[j] = static_cast<std::byte>(pieces[i + j]);

            pieceHashes->push_back(hash);
        }

        return pieceHashes;
    }
}
