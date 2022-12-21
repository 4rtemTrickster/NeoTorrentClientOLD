#pragma once
#include "NTCpch.h"
#include "Torrent/Bencode/BencodeTypes/IBencodeElement.h"
#include "Torrent/Bencode/BencodeTypes/BencodeString/BencodeString.h"

namespace NTC
{
    using BMap = std::unordered_map<BencodeString, Ref<IBencodeElement>, BencodeString>;
    
    class BencodeDictionary : public IBencodeElement
    {
    public:
        using value_type = IBencodeElement;
        
        BencodeDictionary() = default;
        BencodeDictionary(BMap&& map);

        virtual ~BencodeDictionary() override = default;

        inline const BMap& GetValue() const { return Map_; }
        inline BMap& GetValue() { return Map_; }

        Ref<IBencodeElement> operator[](const std::string& key);
        Ref<IBencodeElement> at(const std::string& key);

        inline bool contains(const BMap::key_type& key) const { return Map_.contains(key); }

        virtual std::string Encode() override;

        static Ref<BencodeDictionary> Read(const std::string& encoded, std::string::size_type& index);

    protected:
        BMap Map_;
    };
}
