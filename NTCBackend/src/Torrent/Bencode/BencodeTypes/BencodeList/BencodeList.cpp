#include "NTCpch.h"
#include "BencodeList.h"

#include "Torrent/Bencode/BencodeDecoder/BencodeDecoder.h"

namespace NTC
{
    BencodeList::BencodeList(BList&& list)
        : List_(std::move(list)) {}

    std::string BencodeList::Encode()
    {
        std::string ret;

        ret.append("l");

        for (auto& value : List_)
            ret.append((*value).Encode());

        ret.append("e");

        return ret;
    }

    Ref<BencodeList> BencodeList::Read(const std::string& encoded, std::string::size_type& index)
    {
        if(encoded.at(index) == 'l') ++index;

        BList list;

        while (encoded.at(index) != 'e')
            list.push_back(BencodeDecoder::Decode(encoded, index));

        ++index;

        return CreateRef<BencodeList>(std::move(list));
    }
}
