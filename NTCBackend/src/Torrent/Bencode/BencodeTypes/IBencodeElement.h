#pragma once

namespace NTC
{
    class IBencodeElement
    {
    public:
        virtual ~IBencodeElement() = default;
        virtual std::string Encode() = 0;
    };
}
