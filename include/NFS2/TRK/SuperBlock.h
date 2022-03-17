#pragma once

#include <cstdint>
#include <vector>

#include "Common/Utils.h"
#include "NFS2/Common.h"

#include "NFS2/TRK/TrackBlock.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <Platform platform>
        class SuperBlock : public IDeserializable
        {
        public:
            // Raw file data
            uint32_t superBlockSize;
            uint32_t nBlocks;
            uint32_t padding;
            std::vector<TrackBlock<platform>> trackBlocks;
            std::vector<uint32_t> blockOffsets;

        private:
            void SerializeIn(std::istream &ifstream) override;
        };
    } // namespace NFS2
} // namespace LibOpenNFS
