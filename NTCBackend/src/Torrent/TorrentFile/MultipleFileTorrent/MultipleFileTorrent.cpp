#include "NTCpch.h"
#include "MultipleFileTorrent.h"

namespace NTC
{
    MultipleFileTorrent::file::file(int64_t length, const std::string& md5sum, std::list<std::string>& path)
        : Length_(length), Md5Sum_(md5sum), Path_(path) {}

    MultipleFileTorrent::file::file(int64_t length, std::string&& md5sum, std::list<std::string>&& path)
        : Length_(length), Md5Sum_(std::move(md5sum)), Path_(std::move(path)) {}

    MultipleFileTorrent::file::file(file&& other) noexcept
        : Length_(other.Length_), Md5Sum_(std::move(other.Md5Sum_)), Path_(std::move(other.Path_))
    {
        other.Length_ = 0;
    }
}
