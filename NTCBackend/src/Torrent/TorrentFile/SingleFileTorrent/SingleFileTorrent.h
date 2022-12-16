#pragma once
#include "Torrent/TorrentFile/ITorrentFile.h"

namespace NTC
{
    class SingleFileTorrent : public ITorrentFile
    {
    public:
        using Parent = ITorrentFile;

        SingleFileTorrent(
            const std::string& announce,
            int64_t pieceLength,
            const std::vector<Hash_t>& pieceHashes,
            const std::string& name,
            int64_t length);

        SingleFileTorrent(
            std::string&& announce,
            int64_t pieceLength,
            std::vector<Hash_t>&& pieceHashes,
            std::string&& name,
            int64_t length);

        [[nodiscard]]
        virtual const std::string& GetName() const override { return Name_; }
        
        [[nodiscard]]
        const int64_t& GetLength() const { return Length_; }

        [[nodiscard]]
        const std::string& GetMd5Sum() const { return Md5Sum; }
        

        void SetMd5Sum(const std::string& md5sum);
        void SetMd5Sum(std::string&& md5sum);

        void SetLength(int64_t length);

    protected:
        std::string Name_;
        int64_t Length_;
        std::string Md5Sum;
    };
}
