#pragma once
#include "Torrent/Bencode/BencodeTypes/BencodeDictionary/BencodeDictionary.h"
#include "Torrent/Bencode/BencodeTypes/BencodeList/BencodeList.h"
#include "Torrent/TorrentFile/ITorrentFile.h"
#include "Torrent/TorrentFile/MultipleFileTorrent/MultipleFileTorrent.h"
#include "Torrent/TorrentFile/SingleFileTorrent/SingleFileTorrent.h"

namespace NTC
{
    class TorrentFileFactory
    {
    public:
        TorrentFileFactory() = delete;
        TorrentFileFactory(const TorrentFileFactory&) = delete;
        TorrentFileFactory(TorrentFileFactory&&) = delete;

        [[nodiscard]]
        static Ref<ITorrentFile> CreateTorrentFile(Ref<BencodeDictionary>& dictionary);

    private:
        [[nodiscard]]
        static Ref<MultipleFileTorrent> CreateMultipleFileTorrent(Ref<BencodeDictionary>& InfoDic,
                                                                  Ref<std::string>& announce,
                                                                  Ref<int64_t>& pieceLength,
                                                                  Ref<std::string>& name,
                                                                  Ref<std::vector<Hash_t>>& pieceHashes);

        [[nodiscard]]
        static Ref<SingleFileTorrent> CreateSingleFileTorrent(Ref<BencodeDictionary>& InfoDic,
                                                              Ref<std::string>& announce,
                                                              Ref<int64_t>& pieceLength,
                                                              Ref<std::string>& name,
                                                              Ref<std::vector<Hash_t>>& pieceHashes);
        

        static Ref<std::vector<Hash_t>> SeparatePiecesStr(Ref<std::string>& pieces);

        static inline Ref<std::string> TryGetStringValue(Ref<BencodeDictionary>& dic, const std::string& key);
        static inline Ref<std::string> TryGetStringValue(Ref<IBencodeElement>& element);
        static inline Ref<int64_t> TryGetIntValue(Ref<BencodeDictionary>& dic, const std::string& key);
        static inline Ref<int64_t> TryGetIntValue(Ref<IBencodeElement>& element);
        static inline Ref<BencodeList> TryGetListValue(Ref<BencodeDictionary>& dic, const std::string& key);
        static inline Ref<BencodeDictionary> TryGetDictionaryValue(Ref<BencodeDictionary>& dic, const std::string& key);
        static inline Ref<BencodeDictionary> TryGetDictionaryValue(Ref<IBencodeElement>& element);

        static Ref<IBencodeVisitor> visitor_;
    };
}