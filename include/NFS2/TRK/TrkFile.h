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
        class TrkFile : ISerializable
        {
        public:
            TrkFile() = default;
            static bool Load(const std::string &trkPath, TrkFile &trkFile, NFSVer version);
            static void Save(const std::string &trkPath, TrkFile &trkFile);

            static const uint8_t HEADER_LENGTH         = 4;
            static const uint8_t UNKNOWN_HEADER_LENGTH = 5;

            // ONFS attribute
            NFSVer version;

            // Raw File data
            unsigned char header[HEADER_LENGTH];
            uint32_t unknownHeader[UNKNOWN_HEADER_LENGTH];
            uint32_t nSuperBlocks;
            uint32_t nBlocks;
            std::vector<SuperBlock<Platform>> superBlocks;
            std::vector<uint32_t> superBlockOffsets;
            std::vector<VERT_HIGHP> blockReferenceCoords;

        private:
            bool SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream &ofstream) override;
        };
    } // namespace NFS2
} // namespace LibOpenNFS
