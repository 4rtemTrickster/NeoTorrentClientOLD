#pragma once
#include "Torrent/Bencode/BencodeTypes/IBencodeElement.h"
#include "Torrent/Bencode/BencodeTypes/BencodeString/BencodeString.h"

namespace NTC
{
    using BMap = std::unordered_map<BencodeString, Ref<IBencodeElement>, BencodeString>;
    
    class BencodeDictionary : public IBencodeElement
    {
    public:
        BencodeDictionary() = default;
        BencodeDictionary(BMap&& map);

        virtual ~BencodeDictionary() override = default;

        inline const BMap& GetValue() const { return Map_; }

        virtual std::string Encode() override;

        static Ref<BencodeDictionary> Read(const std::string& encoded, std::string::size_type& index);

    protected:
        BMap Map_;
    };
}
