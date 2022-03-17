#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "Common/Utils.h"
#include "NFS3/Common.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        static constexpr uint8_t cNumPolygonBlocks = 7;
        static constexpr uint8_t cNumPolyObjChunks = 4;

        struct ObjectPolyBlock // a POLYOBJ chunk
        {
            uint32_t n1;                                // total number of polygons
            uint32_t n2;                                // total number of objects including XOBJs
            uint32_t nobj;                              // not stored in .FRD : number of type 1 objects
            std::vector<uint32_t> types;                // when 1, there is an associated object; else XOBJ
            std::vector<uint32_t> numpoly;              // size of each object (only for type 1 objects)
            std::vector<std::vector<PolygonData>> poly; // the polygons themselves
        };

        class PolyBlock : public ISerializable, public IDeserializable
        {
        public:
            uint32_t m_nTrackBlockPolys;

            // 7 blocks == low res / 0 / med. res / 0 / high res / 0 / ??central
            std::array<uint32_t, cNumPolygonBlocks> sz{};
            std::array<uint32_t, cNumPolygonBlocks> szdup{};
            std::array<std::vector<PolygonData>, cNumPolygonBlocks> poly{};
            std::array<ObjectPolyBlock, cNumPolyObjChunks> obj{}; // the POLYOBJ chunks
            // if not present, then all objects in the chunk are XOBJs
            // the 1st chunk is described anyway in the TRKBLOCK

        private:
            void SerializeIn(std::istream &ifstream) override;
            void SerializeOut(std::ostream &ofstream) const override;
        };

    } // namespace NFS3
} // namespace LibOpenNFS
