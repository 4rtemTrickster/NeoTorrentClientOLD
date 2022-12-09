#pragma once
#include "Torrent/Bencode/BencodeTypes/IBencodeElement.h"

namespace NTC
{
    using BList = std::list<Ref<IBencodeElement>>;
    
    class BencodeList : public IBencodeElement
    {
    public:
        BencodeList() = default;
        BencodeList(BList&& list);

        inline const BList& GetValue() const { return List_; }

        virtual std::string Encode() override;

        static Ref<BencodeList> Read(const std::string& encoded, std::string::size_type& index);

    protected:

        BList List_;
    };
}
