#pragma once

#include <cstdint>
#include <vector>

#include "Common/Enums.h"
#include "Common/ISerializable.h"
#include "NFS2/Common.h"
#include "NFS2/TRK/SuperBlock.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <typename Platform>
        class TrkFile : public IDeserializable
        {
        public:
            static const uint8_t HEADER_LENGTH         = 4;
            static const uint8_t UNKNOWN_HEADER_LENGTH = 5;

            // Raw File data
            char header[HEADER_LENGTH];
            uint32_t unknownHeader[UNKNOWN_HEADER_LENGTH];
            uint32_t nSuperBlocks;
            uint32_t nBlocks;
            std::vector<SuperBlock<Platform>> superBlocks;
            std::vector<uint32_t> superBlockOffsets;
            std::vector<VERT_HIGHP> blockReferenceCoords;

        private:
            void SerializeIn(std::istream &ifstream) override;
        };
    } // namespace NFS2
} // namespace LibOpenNFS
