#pragma once

#include <map>
#include <vector>

#include "Common/Utils.h"
#include "NFS2/TRK/ExtraObjectBlock.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <Platform platform>
        class ColFile : public IDeserializable
        {
            ExtraObjectBlock<platform> GetExtraObjectBlock(ExtraBlockID eBlockType);
            bool IsBlockPresent(ExtraBlockID eBlockType);

            static constexpr uint8_t cHeaderLength = 4;

            // Raw File data
            char header[cHeaderLength];
            uint32_t colVersion;
            uint32_t size;
            uint32_t nExtraBlocks;
            std::vector<uint32_t> extraBlockOffsets;
            std::vector<ExtraObjectBlock<platform>> extraObjectBlocks;

        protected:
            void SerializeIn(std::istream &ifstream) override;

            // Allows lookup by block type for parsers
            std::map<ExtraBlockID, uint8_t> extraObjectBlockMap;
        };

    } // namespace NFS2
} // namespace LibOpenNFS
