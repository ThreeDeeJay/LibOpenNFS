#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "NFS5/CRP/BasicTypes.hpp"

namespace LibOpenNFS
{
    namespace NFS5
    {
        namespace CRP
        {
            struct HeaderInfo
            {
                uint32_t : 5; // Unknown (always seems to be 0x1A)
                uint32_t partsNumber : 27;
            };

            /*    The .CRP file starts with following 16-byte header: */
            struct Header
            {
                uint8_t identifier[4]; // cars: ' raC'/'Car'; tracks: 'karT'/'Trak'
                HeaderInfo headerInfo;
                int32_t miscDataCount;  // Number of MiscData following table of Articles
                int32_t articlesOffset; // * 16
            };
            static_assert(sizeof(Header) == 16);

            struct InfoRow
            {
                uint32_t : 32;   // Unknown
                uint32_t offset; // offset in info data
                uint16_t length; // length of info data used;
                uint16_t : 16;   // Unknown
                uint16_t levelIndex;
                uint16_t : 16; // Unknown
            };
            static_assert(sizeof(InfoRow) == 16);

            using VertexRow = InfoRow;
            using NormalRow = InfoRow;
            using UVRow     = InfoRow;

            struct IndexRow
            {
                uint16_t index;
                char identifier[2];
                uint32_t offset;
            };
            static_assert(sizeof(IndexRow) == 8);

            struct Material
            {
                uint32_t : 32; // Unknown
                uint32_t : 32; // Unknown
                uint32_t : 32; // Unknown
                uint32_t : 32; // Unknown
                char description[16];
                uint32_t : 32;             // Unknown
                uint32_t : 32;             // Unknown
                uint32_t texturePageIndex; // see TPG file
                uint32_t _unknown[67];
            };
            static_assert(sizeof(Material) == 312);

            struct LevelInfo
            {
                char info[12];
            };

            struct Base
            {
                uint32_t numberOfLevels;
                uint8_t geometryIndex;
                uint8_t typeIndex;   // 0 = default; other = varation
                uint16_t levelIndex; // LEVIND: IL 00 (with I the index: 1 = body; 2 = wheel; 8 = driver)
                std::vector<LevelInfo> levelMasks;
            };

            struct Culling
            {
                uint32_t numberOfCulling;
            };

            struct Triangle
            {
                /* Info flags:
                   1st byte: D3DFILLMODE (D3DFILL_POINT = 1, D3DFILL_WIREFRAME = 2, D3DFILL_SOLID = 3) */
                uint32_t infoFlags; // type is unknown
                uint16_t materialIndex;
                uint16_t : 16;
                float _unknown[4];
                uint32_t _unknown2[4];      // always seems to be 0x00000000
                uint32_t numberOfInfoRows;  // NIF
                uint32_t numberOfIndexRows; // NID
                /*  0x30    LENF InfoRow[NIF]  Info rows
                    OFFD    LEND IndexRow[NID] Index rows

                    LENF = NIF * 0x10
                    OFFD = LENF + 0x30
                    LEND = NID * 0x08 */
            };

            struct Effect
            {
                uint32_t : 32; // Unknown (seems always to be 0x05000000)
                uint32_t : 32; // Unknown (seems always to be 0x00000000)
                Vector3f position;
                uint32_t : 32;   // Unknown (seems always to be 0x3F800000)
                Vector3f width;  // relative to position
                uint32_t : 32;   // Unknown (seems always to be 0x00000000)
                Vector3f height; // relative to position
                uint32_t : 32;   // Unknown (seems always to be 0x00000000)
                Vector3f depth;  // relative to position
                uint32_t : 32;   // Unknown (seems always to be 0x00000000)
                ColorBGRA glowColor;
                ColorBGRA sourceColor;
                uint32_t mirror; // 0x08000000 means mirrored
                uint32_t information;
            };
            static_assert(sizeof(Effect) == 88);

            using TransformationMatrix = Matrix4f;

            // Vertex seems to be a normal homogeneous 3D point.
            using Vertex = Vector4f;

            // Normal seems to be a normal homogeneous 3D vector.
            using Normal = Vector4f;

            using UV = Vector2f;

            struct Article
            {
                HeaderInfo headerInfo;

                std::vector<Base> bases;
                std::vector<std::string> names;
                std::vector<Culling> cullings;
                std::vector<TransformationMatrix> transformationMatrices;
                std::vector<Vertex> vertices;
                std::vector<Normal> normals;
                std::vector<UV> uvs;
                std::vector<Triangle> triangles;
                std::vector<Effect> effects;
            };
        } // namespace CRP
    }     // namespace NFS5
} // namespace LibOpenNFS