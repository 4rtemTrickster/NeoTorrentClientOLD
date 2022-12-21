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
        
        template <Derived<IBencodeElement> T>
        static boost::optional<typename T::value_type&> TryGetValue(Ref<BencodeDictionary>& dic, const std::string& key);
    };

    template <Derived<IBencodeElement> T>
    boost::optional<typename T::value_type&> TorrentFileFactory::TryGetValue(Ref<BencodeDictionary>& dic,
                                                                             const std::string& key)
    {
        if(auto elem = dic->at(key))
        {
            if(Ref<T> casted = DynamicCast<T>(elem))
                return boost::optional<typename T::value_type&>(casted->GetValue());
            else
                NTC_WARN("Element by key: " + key + " incorrect cast type");
        }
        else
            NTC_WARN("Element by key:" + key + " does not exist");

        return {};
    }
}
