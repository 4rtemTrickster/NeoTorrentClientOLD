#include "NTCpch.h"
#include "TorrentFileFactory.h"

#include "Torrent/TorrentFile/MultipleFileTorrent/MultipleFileTorrent.h"
#include "Torrent/TorrentFile/SingleFileTorrent/SingleFileTorrent.h"

#include "Torrent/Bencode/BencodeTypes/BencodeInt/BencodeInt.h"
#include "Torrent/Bencode/BencodeTypes/BencodeList/BencodeList.h"
#include "Torrent/Bencode/BencodeVisitor/BencodeVisitor.h"

namespace NTC
{
    Ref<IBencodeVisitor> TorrentFileFactory::visitor_ = CreateRef<BencodeVisitor>();

    static std::future<void> stringSeparationResult;

    Ref<ITorrentFile> TorrentFileFactory::CreateTorrentFile(Ref<BencodeDictionary>& dictionary)
    {
        NTC_PROFILE_FUNCTION();
        
        Ref<BencodeDictionary> InfoDic = TryGetDictionaryValue(dictionary, "info");

        if (!InfoDic)
        {
            NTC_ERROR("Can not find info dictionary while parsing");
            return {};
        }
        
        Ref<std::string> announce = TryGetStringValue(dictionary, "announce");
        Ref<int64_t> pieceLength = TryGetIntValue(InfoDic, "piece length");
        Ref<std::string> pieces = TryGetStringValue(InfoDic, "pieces");
        Ref<std::string> name = TryGetStringValue(InfoDic, "name");
        Ref<std::vector<Hash_t>> piecesHashes;
        if (pieces != nullptr)
            stringSeparationResult = std::async(std::launch::async, [&](){piecesHashes = SeparatePiecesStr(pieces); });
        

        if (InfoDic->contains("files"))
            return CreateMultipleFileTorrent(InfoDic, announce, pieceLength, name, piecesHashes);
        else
            return CreateSingleFileTorrent(InfoDic, announce, pieceLength, name, piecesHashes);
    }


    Ref<MultipleFileTorrent> TorrentFileFactory::CreateMultipleFileTorrent(Ref<BencodeDictionary>& InfoDic,
                                                                             Ref<std::string>& announce,
                                                                             Ref<int64_t>& pieceLength,
                                                                             Ref<std::string>& name,
                                                                             Ref<std::vector<Hash_t>>& pieceHashes)
    {
        NTC_PROFILE_FUNCTION();
        Ref<BencodeList> FilesList = TryGetListValue(InfoDic, "files");

        std::list<MultipleFileTorrent::file> files;
        for (auto& element : *FilesList)
        {
            NTC_PROFILE_SCOPE("FilesList");
            Ref<BencodeDictionary> dic = TryGetDictionaryValue(element);
            Ref<int64_t> length = TryGetIntValue(dic, "length");

            Ref<BencodeList> pathList = TryGetListValue(dic, "path");
            std::list<std::string> path;

            for (auto& pathPart : *pathList)
            {
                NTC_PROFILE_SCOPE("paths");
                Ref<std::string> pp = TryGetStringValue(pathPart);

                if (pp != nullptr)
                    path.push_back(*pp);
                else
                    NTC_ERROR("Path part value error");
            }

            if (length != nullptr)
                files.emplace_back(*length, std::move(path));
            else
            {
                NTC_ERROR(
                    "Cannot create a torrent file representation 'cause one of the required fields is not filled in");
                return nullptr;
            }
        }

        stringSeparationResult.wait();

        if (announce != nullptr &&
            pieceLength != nullptr &&
            name != nullptr &&
            pieceHashes != nullptr &&
            !files.empty()
        )
            return CreateRef<MultipleFileTorrent>(
                std::move(*announce),
                *pieceLength,
                std::move(pieceHashes),
                std::move(*name),
                std::move(files)
            );

        NTC_ERROR("Cannot create a torrent file representation 'cause one of the required fields is not filled in");
        
        return {};
    }

    Ref<SingleFileTorrent> TorrentFileFactory::CreateSingleFileTorrent(Ref<BencodeDictionary>& InfoDic,
                                                                         Ref<std::string>& announce,
                                                                         Ref<int64_t>& pieceLength,
                                                                         Ref<std::string>& name,
                                                                         Ref<std::vector<Hash_t>>& pieceHashes)
    {
        NTC_PROFILE_FUNCTION();
        const Ref<int64_t> length = TryGetIntValue(InfoDic, "length");

        stringSeparationResult.wait();

        if (announce != nullptr &&
            pieceLength != nullptr &&
            name != nullptr &&
            pieceHashes != nullptr &&
            length != nullptr)
            return CreateRef<SingleFileTorrent>(
                std::move(*announce),
                *pieceLength,
                std::move(pieceHashes),
                std::move(*name),
                *length);

        NTC_ERROR("Cannot create a torrent file representation 'cause one of the required fields is not filled in");

        return {};
    }

    Ref<std::vector<Hash_t>> TorrentFileFactory::SeparatePiecesStr(Ref<std::string>& pieces)
    {
        NTC_PROFILE_FUNCTION();

        //TODO: YOU NEED to optimize this shit! we a copying almost full .torrent file
        Ref<std::vector<Hash_t>> pieceHashes = CreateRef<std::vector<Hash_t>>(pieces->size() / 20);

        std::memcpy(pieceHashes->data(), pieces->data(), pieces->size());

        return pieceHashes;
    }

    Ref<std::string> TorrentFileFactory::TryGetStringValue(Ref<BencodeDictionary>& dic, const std::string& key)
    {
        if (dic->contains(key))
        {
            dic->at(key)->Accept(visitor_);
            return CreateRef<std::string>(visitor_->GetStringValue()->GetValue());
        }
        else
            NTC_WARN("Element by key: " + key + " does not exist");

        return {};
    }

    Ref<std::string> TorrentFileFactory::TryGetStringValue(Ref<IBencodeElement>& element)
    {
        element->Accept(visitor_);
        return CreateRef<std::string>(visitor_->GetStringValue()->GetValue());
    }

    Ref<int64_t> TorrentFileFactory::TryGetIntValue(Ref<BencodeDictionary>& dic, const std::string& key)
    {
        if (dic->contains(key))
        {
            dic->at(key)->Accept(visitor_);
            return CreateRef<int64_t>(visitor_->GetIntValue()->GetValue());
        }
        else
            NTC_WARN("Element by key: " + key + " does not exist");

        return {};
    }

    Ref<int64_t> TorrentFileFactory::TryGetIntValue(Ref<IBencodeElement>& element)
    {
        element->Accept(visitor_);
        return CreateRef<int64_t>(visitor_->GetIntValue()->GetValue());
    }

    Ref<BencodeList> TorrentFileFactory::TryGetListValue(Ref<BencodeDictionary>& dic, const std::string& key)
    {
        if (dic->contains(key))
        {
            dic->at(key)->Accept(visitor_);
            return visitor_->GetListValue();
        }
        else
            NTC_WARN("Element by key: " + key + " does not exist");

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
        else
            NTC_WARN("Element by key:" + key + " does not exist");

        return {};
    }

    Ref<BencodeDictionary> TorrentFileFactory::TryGetDictionaryValue(Ref<IBencodeElement>& element)
    {
        element->Accept(visitor_);
        return visitor_->GetDictionaryValue();
    }
}