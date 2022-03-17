#pragma once

#include <cstdint>
#include <vector>

#include "Common/ISerializable.h"
#include "NFS2/Common.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <typename Platform>
        class StructureBlock : private ISerializable
        {
        public:
            StructureBlock() = default;
            explicit StructureBlock(std::istream &ifstream);
            void SerializeOut(std::ostream &ofstream) override;

            uint32_t recSize;
            uint16_t nVerts;
            uint16_t nPoly;
            std::vector<typename Platform::VERT> vertexTable;
            std::vector<typename Platform::POLYGONDATA> polygonTable;

        private:
            bool SerializeIn(std::istream &ifstream) override;
        };

    } // namespace NFS2
} // namespace LibOpenNFS
