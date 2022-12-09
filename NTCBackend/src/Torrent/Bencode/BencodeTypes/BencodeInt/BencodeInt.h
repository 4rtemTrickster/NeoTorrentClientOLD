#pragma once
#include "Torrent/Bencode/BencodeTypes/IBencodeElement.h"

namespace NTC
{
    class BencodeInt : public IBencodeElement
    {
    public:
        BencodeInt() : value_(0) {}
        BencodeInt(int value) : value_(value) {}

        inline int GetValue() const { return value_; }

        virtual std::string Encode() override;

        static Ref<BencodeInt> Read(const std::string& encoded, std::string::size_type& index);

    protected:
        int value_;
    };
}