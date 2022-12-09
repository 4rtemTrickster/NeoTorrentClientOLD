#include "NTCpch.h"
#include "BencodeString.h"

namespace NTC
{
    std::string BencodeString::Encode()
    {
        return std::to_string(value_.length()) + ":" + value_;
    }

    Ref<BencodeString> BencodeString::Read(const std::string& encoded, std::string::size_type& index)
    {
        std::string::size_type colonIndex = encoded.find_first_of(':', index);
        std::string::size_type length = std::stoi(std::string_view(encoded.c_str() + index, colonIndex).data());

        index = colonIndex + 1;

        return CreateRef<BencodeString>(std::string_view(encoded.c_str() + index, length));
    }
}
