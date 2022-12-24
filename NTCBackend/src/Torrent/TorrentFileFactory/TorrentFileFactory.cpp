#include "NTCpch.h"
#include "TorrentFileFactory.h"

#include "Torrent/TorrentFile/MultipleFileTorrent/MultipleFileTorrent.h"
#include "Torrent/TorrentFile/SingleFileTorrent/SingleFileTorrent.h"

#include "Torrent/Bencode/BencodeTypes/BencodeInt/BencodeInt.h"
#include "Torrent/Bencode/BencodeTypes/BencodeList/BencodeList.h"
#include "Torrent/Bencode/BencodeVisitor/BencodeVisitor.h"

template <class T>
using optional = boost::optional<T>;

namespace NTC
{
    Ref<IBencodeVisitor> TorrentFileFactory::visitor_ = CreateRef<BencodeVisitor>();

    Ref<ITorrentFile> TorrentFileFactory::CreateTorrentFile(Ref<BencodeDictionary>& dictionary)
    {
        Ref<BencodeDictionary> InfoDic = TryGetDictionaryValue(dictionary, "info");

        if (!InfoDic)
        {
            NTC_ERROR("Can not find info dictionary while parsing");
            return {};
        }

        optional<std::string&> announce = TryGetStringValue(dictionary, "announce");
        optional<int64_t&> pieceLength = TryGetIntValue(dictionary, "pieceLength");
        optional<std::string&> pieces = TryGetStringValue(dictionary, "pieces");
        optional<std::string&> name = TryGetStringValue(InfoDic, "name");
        Ref<std::vector<Hash_t>> piecesHashes;
        if (pieces.has_value()) piecesHashes = SeparatePiecesStr(pieces.value());


        Ref<ITorrentFile> toReturn;

        if (InfoDic->contains("files"))
        {
            Ref<BencodeList> FilesList = DynamicCast<BencodeList>(InfoDic->at("files"));

            std::list<MultipleFileTorrent::file> files;
            for (auto& element : *FilesList)
            {
                Ref<BencodeDictionary> dic = TryGetDictionaryValue(element);
                optional<int64_t&> length = TryGetIntValue(dic, "length");

                Ref<BencodeList> pathList = TryGetListValue(dic, "path");
                std::list<std::string> path;

                for (auto& pathPart : *pathList)
                {
                    optional<std::string&> pp = TryGetStringValue(pathPart);

                    if (pp.has_value())
                        path.push_back(pp.value());
                    else
                        NTC_ERROR("Path part value error");
                }

                if (length.has_value())
                    files.emplace_back(*length, std::move(path));
                else
                {
                    NTC_ERROR(
                        "Cannot create a torrent file representation 'cause one of the required fields is not filled in");
                    return nullptr;
                }
            }

            if (announce.has_value() &&
                pieceLength.has_value() &&
                name.has_value() &&
                pieces.has_value() &&
                piecesHashes != nullptr &&
                !files.empty()
            )
                toReturn = CreateRef<MultipleFileTorrent>(
                    announce.value(),
                    pieceLength.value(),
                    *piecesHashes,
                    name.value(), files
                );
            else
            {
                NTC_ERROR(
                    "Cannot create a torrent file representation 'cause one of the required fields is not filled in");
                return nullptr;
            }
        }
        else
        {
            optional<int64_t&> length = TryGetIntValue(InfoDic, "length");

            if (announce.has_value() &&
                pieceLength.has_value() &&
                name.has_value() &&
                pieces.has_value()
                && length.has_value()
            )
                toReturn = CreateRef<SingleFileTorrent>(
                    announce.value(),
                    pieceLength.value(),
                    *piecesHashes,
                    name.value(),
                    length.value());
            else
            {
                NTC_ERROR(
                    "Cannot create a torrent file representation 'cause one of the required fields is not filled in");
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
                hash.at(j) = static_cast<std::byte>(pieces.at(i + j));

            pieceHashes->push_back(hash);
        }

        return pieceHashes;
    }

    optional<std::string&> TorrentFileFactory::TryGetStringValue(Ref<BencodeDictionary>& dic, const std::string& key)
    {
        if (dic->contains(key))
        {
            dic->at(key)->Accept(visitor_);
            return visitor_->GetStringValue()->GetValue();
        }

        return {};
    }

    boost::optional<std::string&> TorrentFileFactory::TryGetStringValue(Ref<IBencodeElement>& element)
    {
        element->Accept(visitor_);
        return visitor_->GetStringValue()->GetValue();
    }

    optional<int64_t&> TorrentFileFactory::TryGetIntValue(Ref<BencodeDictionary>& dic, const std::string& key)
    {
        if (dic->contains(key))
        {
            dic->at(key)->Accept(visitor_);
            return visitor_->GetIntValue()->GetValue();
        }

        return {};
    }

    boost::optional<int64_t&> TorrentFileFactory::TryGetIntValue(Ref<IBencodeElement>& element)
    {
        element->Accept(visitor_);
        return visitor_->GetIntValue()->GetValue();
    }

    Ref<BencodeList> TorrentFileFactory::TryGetListValue(Ref<BencodeDictionary>& dic, const std::string& key)
    {
        if (dic->contains(key))
        {
            dic->at(key)->Accept(visitor_);
            return visitor_->GetListValue();
        }

        return {};
    }

    Ref<BencodeDictionary> TorrentFileFactory::TryGetDictionaryValue(Ref<BencodeDictionary>& dic,
                                                                     const std::string& key)
    {
        if (dic->contains(key))
        {
            dic->at(key)->Accept(visitor_);
            return visitor_->GetDictionaryValue();
        }

        return {};
    }

    Ref<BencodeDictionary> TorrentFileFactory::TryGetDictionaryValue(Ref<IBencodeElement>& element)
    {
        element->Accept(visitor_);
        return visitor_->GetDictionaryValue();
    }
}
