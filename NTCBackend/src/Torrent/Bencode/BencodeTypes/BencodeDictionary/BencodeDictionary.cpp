#include "NTCpch.h"
#include "BencodeDictionary.h"

#include "Torrent/Bencode/BencodeDecoder/BencodeDecoder.h"

namespace NTC
{
    BencodeDictionary::BencodeDictionary(BMap&& map)
        : Map_(std::move(map)) {}

    std::string BencodeDictionary::Encode()
    {
        std::string ret;
        
        ret.append("d");
        for (auto& [key, value] : Map_)
            ret.append(key->Encode() + value->Encode());

        ret.append("e");

        return ret;
    }

    Ref<BencodeDictionary> BencodeDictionary::Read(const std::string& encoded, std::string::size_type& index)
    {
        if (encoded.at(index) == 'd') ++index;

        BMap map;

        while (encoded.at(index) != 'e')
        {
            Ref<BencodeString> key = BencodeString::Read(encoded,index);
            Ref<IBencodeElement> value = BencodeDecoder::Decode(encoded, index);
            map.emplace(key, value);
        }

        ++index;

        return CreateRef<BencodeDictionary>(std::move(map));
    }
}
