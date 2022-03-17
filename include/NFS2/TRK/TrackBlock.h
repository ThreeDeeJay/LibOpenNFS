#pragma once

#include <cstdint>
#include <vector>
#include <map>

#include "Common/Enums.h"
#include "Common/ISerializable.h"
#include "NFS2/Common.h"
#include "NFS2/TRK/ExtraObjectBlock.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <typename Platform>
        class TrackBlock : public IDeserializable
        {
        public:
            ExtraObjectBlock<Platform> GetExtraObjectBlock(ExtraBlockID eBlockType);
            bool IsBlockPresent(ExtraBlockID eBlockType);

            // Raw file data
            uint32_t blockSize;
            uint32_t blockSizeDup;
            uint16_t nExtraBlocks;
            uint16_t unknown;
            uint32_t serialNum;
            struct VERT_HIGHP clippingRect[4];
            uint32_t extraBlockTblOffset;
            uint16_t nStickToNextVerts, nLowResVert, nMedResVert, nHighResVert;
            uint16_t nLowResPoly, nMedResPoly, nHighResPoly;
            uint16_t unknownPad[3];
            std::vector<typename Platform::VERT> vertexTable;
            std::vector<typename Platform::POLYGONDATA> polygonTable;
            std::vector<uint32_t> extraBlockOffsets;
            std::vector<ExtraObjectBlock<Platform>> extraObjectBlocks;

        protected:
            void SerializeIn(std::istream &ifstream) override;

            // Allows lookup by block type for parsers
            std::map<ExtraBlockID, uint8_t> extraObjectBlockMap;
        };
    } // namespace NFS2
} // namespace LibOpenNFS
