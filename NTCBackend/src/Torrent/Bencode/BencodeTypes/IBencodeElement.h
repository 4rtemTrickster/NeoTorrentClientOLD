#pragma once
#include "Torrent/Bencode/BencodeVisitor/IBencodeVisitor.h"

namespace NTC
{
    using BList = std::list<Ref<IBencodeElement>>;
    using BMap = std::unordered_map<BencodeString, Ref<IBencodeElement>, BencodeString>;
    
    class IBencodeElement
    {
    public:
        using value_type = IBencodeElement;
        virtual ~IBencodeElement() = default;
        virtual std::string Encode() = 0;

        virtual void Accept(Ref<IBencodeVisitor>& visitor) = 0;
    };
}
