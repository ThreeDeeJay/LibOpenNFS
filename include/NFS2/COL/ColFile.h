#pragma once

#include <map>
#include <vector>

#include "Common/Enums.h"
#include "Common/ISerializable.h"
#include "NFS2/TRK/ExtraObjectBlock.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <typename Platform>
        class ColFile : public ISerializable
        {
        public:
            ColFile(const std::string &colPath);
            void Save(const std::string &colPath);

            ExtraObjectBlock<Platform> GetExtraObjectBlock(ExtraBlockID eBlockType);
            bool IsBlockPresent(ExtraBlockID eBlockType);

            static constexpr uint8_t cHeaderLength = 4;

            // Raw File data
            char header[cHeaderLength];
            uint32_t colVersion;
            uint32_t size;
            uint32_t nExtraBlocks;
            std::vector<uint32_t> extraBlockOffsets;
            std::vector<ExtraObjectBlock<Platform>> extraObjectBlocks;

        protected:
            bool SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream &ofstream) override;

            // Allows lookup by block type for parsers
            std::map<ExtraBlockID, uint8_t> extraObjectBlockMap;
        };

    } // namespace NFS2
} // namespace LibOpenNFS
