#pragma once

namespace NTC
{
    class IBencodeElement
    {
    public:
        using value_type = IBencodeElement;
        virtual ~IBencodeElement() {}
        virtual std::string Encode() = 0;
    };
}
