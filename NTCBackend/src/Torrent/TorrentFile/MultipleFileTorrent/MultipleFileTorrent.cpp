﻿#include "NTCpch.h"
#include "MultipleFileTorrent.h"

namespace NTC
{
    MultipleFileTorrent::file::file(int64_t length, std::list<std::string>& path)
        : Length_(length), Path_(path) {}

    MultipleFileTorrent::file::file(int64_t length, std::list<std::string>&& path)
        : Length_(length), Path_(std::move(path)) {}

    MultipleFileTorrent::file::file(file&& other) noexcept
        : Length_(other.Length_), Md5Sum_(std::move(other.Md5Sum_)), Path_(std::move(other.Path_))
    {
        other.Length_ = 0;
    }

    MultipleFileTorrent::MultipleFileTorrent(const std::string& announce, int64_t pieceLength,
        const std::vector<Hash_t>& pieceHashes, const std::string& name, const std::list<file>& files)
            : Parent(announce, pieceHashes, pieceLength),
              Name_(name), Files_(files) {}

    MultipleFileTorrent::MultipleFileTorrent(std::string&& announce, int64_t pieceLength,
        std::vector<Hash_t>&& pieceHashes, std::string&& name, std::list<file>&& files)
            : Parent(std::move(announce), std::move(pieceHashes), pieceLength),
              Name_(std::move(name)), Files_(std::move(files)) {}
}