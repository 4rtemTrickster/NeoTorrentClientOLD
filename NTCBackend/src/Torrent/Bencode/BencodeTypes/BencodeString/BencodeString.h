#pragma once
#include "Torrent/Bencode/BencodeTypes/IBencodeElement.h"

namespace NTC
{
    class BencodeString : public IBencodeElement
    {
    public:
        BencodeString() : value_(std::string()) {}

        BencodeString(const std::string& value) : value_(value) {}

        inline const std::string& GetValue() const { return value_; }
        virtual std::string Encode() override;

        static Ref<BencodeString> Read(const std::string& encoded, std::string::size_type& index);

    protected:
        std::string value_;
    };
}
