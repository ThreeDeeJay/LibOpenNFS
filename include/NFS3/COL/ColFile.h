#pragma once

#include <vector>

#include "Common/Utils.h"
#include "NFS3/Common.h"

namespace LibOpenNFS
{
    namespace NFS3
    {
        enum class XBID : uint16_t
        {
            TextureInfo = 2,
            Struct3D    = 8,
            Object      = 7,
            Object2     = 18,
            VRoad       = 15
        };

        struct ExtraBlockHeader
        {
            uint32_t size;
            XBID xbid;
            uint16_t nrec;
        };

        struct ColTextureInfo
        {
            uint16_t id;       // position in .QFS file
            uint16_t unknown1; // zero ?
            uint16_t unknown2; // texture offset ?
            uint16_t unknown3;
        };

        struct ColVertex
        {
            glm::vec3 pt;     // relative coord
            uint32_t unknown; // like the unknVertices structures in FRD
        };

        struct ColPolygon
        {
            uint16_t texture;
            char v[4]; // vertices
        };

        struct ColStruct3D
        {
            uint32_t size;
            uint16_t nVert, nPoly;
            std::vector<ColVertex> vertex;
            std::vector<ColPolygon> polygon;
        };

        struct ColObject
        {
            uint16_t size;
            uint8_t type;     // 1 = basic object, 3 = animated ...
            uint8_t struct3D; // reference in previous block
            // type 1
            glm::ivec3 ptRef;
            // type 3
            uint16_t animLength;
            uint16_t unknown;
            std::vector<AnimData> animData; // same structure as in xobjs
        };

        struct ColVRoad
        {
            glm::ivec3 refPt;
            uint32_t unknown; // Unknown data
            glm::i8vec4 normal, forward, right;
            uint32_t leftWall, rightWall;
        };

        class ColFile : public IDeserializable
        {
        public:
            char header[4];                      // Header of file 'COLL'
            uint32_t version;                    // Version number 11
            uint32_t fileLength;                 // File length in bytes
            uint32_t nBlocks;                    // Number of Xtra blocks in file
            uint32_t xbTable[5];                 // Offsets of Xtra blocks
            ExtraBlockHeader textureHead = {};   // Record detailing texture table data
            std::vector<ColTextureInfo> texture; // Texture table
            ExtraBlockHeader struct3DHead = {};  // Record detailing struct3D table data
            std::vector<ColStruct3D> struct3D;   // Struct 3D table
            ExtraBlockHeader objectHead = {};    // Record detailing object table data
            std::vector<ColObject> object;       // Object table
            ExtraBlockHeader object2Head = {};   // Record detailing extra object data
            std::vector<ColObject> object2;      // Extra object data
            ExtraBlockHeader vroadHead = {};     // Unknown Record detailing unknown table data
            std::vector<ColVRoad> vroad;         // Unknown table
            uint32_t *hs_extra = nullptr;        // for the extra HS data in ColVRoad

        protected:
            void SerializeIn(std::istream &ifstream) override;
        };
    } // namespace NFS3
} // namespace LibOpenNFS
