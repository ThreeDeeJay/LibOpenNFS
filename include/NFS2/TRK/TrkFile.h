#pragma once

#include <cstdint>
#include <vector>

#include "Common/Utils.h"
#include "NFS2/Common.h"

#include "NFS2/TRK/SuperBlock.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <Platform platform>
        class TrkFile : public IDeserializable
        {
        public:
            static constexpr uint8_t cHeaderLength        = 4;
            static constexpr uint8_t cUnknownHeaderLength = 5;

            // Raw File data
            char header[cHeaderLength];
            uint32_t unknownHeader[cUnknownHeaderLength];
            uint32_t nSuperBlocks;
            uint32_t nBlocks;
            std::vector<SuperBlock<platform>> superBlocks;
            std::vector<uint32_t> superBlockOffsets;
            std::vector<VertexHighP> blockReferenceCoords;

        private:
            void SerializeIn(std::istream &ifstream) override;
        };
    } // namespace NFS2
} // namespace LibOpenNFS
