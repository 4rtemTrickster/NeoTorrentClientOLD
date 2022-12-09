#include "NTCpch.h"
#include "BencodeInt.h"

namespace NTC
{
    std::string BencodeInt::Encode()
    {
        return "i" + std::to_string(value_) + "e";
    }

    Ref<BencodeInt> BencodeInt::Read(const std::string& encoded, std::string::size_type& index)
    {
        if(encoded.at(index) == 'i') ++index;

        std::string::size_type end = encoded.find_first_of('e', index);

        int val = std::stoi(std::string_view(encoded.c_str() + index, end).data());

        index = end + 1;

        return CreateRef<BencodeInt>(val);
    }
}
