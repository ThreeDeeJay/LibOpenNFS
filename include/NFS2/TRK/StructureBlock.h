#pragma once

#include <cstdint>
#include <vector>

#include "Common/Utils.h"
#include "NFS2/Common.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        template <Platform platform>
        class StructureBlock : public IDeserializable
        {
        public:
            uint32_t recSize;
            uint16_t nVerts;
            uint16_t nPoly;
            
            std::vector<Vertex<platform>> vertexTable;
            std::vector<PolygonData<platform>> polygonTable;

        protected:
            void SerializeIn(std::istream &ifstream) override;
        };

    } // namespace NFS2
} // namespace LibOpenNFS
