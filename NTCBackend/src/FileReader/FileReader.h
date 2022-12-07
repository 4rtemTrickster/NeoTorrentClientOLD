#pragma once

namespace NTC
{
    class FileReader
    {
    public:
        FileReader() = delete;

        [[nodiscard]]
        static std::string ReadFile(const std::filesystem::path& path);

    protected:
        
        static std::fstream fs_;
    };
}
