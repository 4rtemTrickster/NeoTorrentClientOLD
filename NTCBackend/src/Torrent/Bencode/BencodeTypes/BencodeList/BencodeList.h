#pragma once
#include "Torrent/Bencode/BencodeTypes/IBencodeElement.h"

namespace NTC
{
    class BencodeList : public IBencodeElement
    {
    public:
        using value_type = IBencodeElement;
        using iterator = BList::iterator;
        using const_iterator = BList::const_iterator;
        
        BencodeList() = default;
        BencodeList(BList&& list);
        virtual ~BencodeList() override = default;

        inline const BList& GetValue() const { return List_; }
        inline BList& GetValue() { return List_; }

        virtual std::string Encode() override;

        virtual void Accept(Ref<IBencodeVisitor>& visitor) override;

        inline iterator begin() noexcept { return List_.begin(); }
        inline const_iterator begin() const noexcept { return List_.cbegin(); }

        inline iterator end() noexcept { return List_.end(); }
        inline const_iterator end() const noexcept { return List_.cend(); }

        static Ref<BencodeList> Read(const std::string& encoded, std::string::size_type& index);

    protected:

        BList List_;
    };
}