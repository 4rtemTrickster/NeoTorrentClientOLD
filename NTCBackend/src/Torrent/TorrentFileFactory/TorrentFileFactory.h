#pragma once
#include "Torrent/Bencode/BencodeTypes/BencodeDictionary/BencodeDictionary.h"
#include "Torrent/Bencode/BencodeTypes/BencodeList/BencodeList.h"
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
        static Ref<ITorrentFile> CreateTorrentFile(Ref<BencodeDictionary>& dictionary);

    private:
        static Ref<std::vector<Hash_t>> SeparatePiecesStr(std::string& pieces);
        
        static inline boost::optional<std::string&> TryGetStringValue(Ref<BencodeDictionary>& dic, const std::string& key);
        static inline boost::optional<std::string&> TryGetStringValue(Ref<IBencodeElement>& element);
        static inline boost::optional<int64_t&> TryGetIntValue(Ref<BencodeDictionary>& dic, const std::string& key);
        static inline boost::optional<int64_t&> TryGetIntValue(Ref<IBencodeElement>& element);
        static inline Ref<BencodeList> TryGetListValue(Ref<BencodeDictionary>& dic, const std::string& key);
        static inline Ref<BencodeDictionary> TryGetDictionaryValue(Ref<BencodeDictionary>& dic, const std::string& key);
        static inline Ref<BencodeDictionary> TryGetDictionaryValue(Ref<IBencodeElement>& element);

        static Ref<IBencodeVisitor> visitor_;
    };
}
