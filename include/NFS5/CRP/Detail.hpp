#pragma once

#include <cstdint>

#include "NFS5/CRP/Types.hpp"

namespace LibOpenNFS
{
    namespace NFS5
    {
        namespace CRP
        {
            namespace Detail
            {
                struct ArticleRead // Each Article points to a PartTable.
                {
                    char identifier[4]; // 'itrA'/'Arti'
                    HeaderInfo headerInfo;

                    int32_t partTableLength; // Length of Part table pointed to (* 16)
                    int32_t offset;          // Relative from current Article offset * 16
                };
                static_assert(sizeof(ArticleRead) == 16);

                struct BaseRead
                {
                    uint32_t : 32; // Unknown
                    uint32_t : 32; // Unknown
                    uint32_t : 32; // Unknown
                    uint32_t numberOfLevels;
                    uint32_t _unknown[13]; // Unknown
                    uint8_t geometryIndex;
                    uint8_t typeIndex;   // 0 = default; other = varation
                    uint16_t : 16;       // Unknown
                    uint16_t : 16;       // Unknown (seems to be unused)
                    uint16_t : 16;       // Unknown
                    uint16_t : 16;       // Unknown (seems to be 01 00)
                    uint16_t levelIndex; // LEVIND: IL 00 (with I the index: 1 = body; 2 = wheel; 8 = driver)
                    uint32_t _unknown2[5];
                };

                struct CullingRead
                {
                    Vector3f coordinate;

                    /*  Positive values: sooner culling
                        Zero value     : normal culling
                        Negative values: later culling */
                    float threshold;
                };
                static_assert(sizeof(CullingRead) == 16);

            } // namespace Detail
        }     // namespace CRP
    }         // namespace NFS5
} // namespace LibOpenNFS
