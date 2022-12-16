#pragma once

namespace NTC
{
    class IBencodeElement
    {
    public:
        virtual ~IBencodeElement() {}
        virtual std::string Encode() = 0;
    };
}
