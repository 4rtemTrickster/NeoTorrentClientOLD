#pragma once
#include "Torrent/TorrentFile/ITorrentFile.h"

namespace NTC
{
    
    class MultipleFileTorrent : public ITorrentFile
    {
    protected:
        struct file
        {
            int64_t Length_;
            std::string Md5Sum_;
            std::list<std::string> Path_;

            file(int64_t length, const std::string& md5sum, std::list<std::string>& path);
            file(int64_t length, std::string&& md5sum, std::list<std::string>&& path);

            file(const file& other) = default;
            file(file&& other) noexcept;
        };
        
    public:
        MultipleFileTorrent(
            const std::string& announce,
            int64_t pieceLength,
            const std::vector<Hash_t>& pieceHashes,
            const std::string& name,
            const file& files);

        MultipleFileTorrent(
            std::string&& announce,
            int64_t pieceLength,
            std::vector<Hash_t>&& pieceHashes,
            std::string&& name,
            file&& files);

        [[nodiscard]]
        virtual const std::string& GetName() const override { return Name_; }
    
    protected:
        
        std::string Name_;
        std::list<file> Files_;
    };
}
