#pragma once

#include <cstdint>

namespace LibOpenNFS
{
    namespace NFS5
    {
        namespace CRP
        {
            enum class PartIdentifier
            {
                eBasePart = 0x65736142,
                eNamePart = 0x656D614E,

                eCullingPart        = 0x246E,
                eTransformationPart = 0x7274,
                eVertexPart         = 0x7476,
                eNormalPart         = 0x6D6E,
                eUVPart             = 0x7675,
                eTrianglePart       = 0x7270,
                eEffectPart         = 0x6665
            };

            struct PartInfoOne
            {
                uint16_t levelOfDetail : 4;
                uint16_t animationIndex : 8;
                uint16_t damageSwitch : 4; // 0x8 means damaged
            };
            static_assert(sizeof(PartInfoOne) == 2);

            struct PartInfoTwo
            {
                uint16_t partIndex : 12;
                uint16_t levelOfDetail : 4;
            };
            static_assert(sizeof(PartInfoTwo) == 2);

            struct PartWithoutInfo
            {
                char identifier[4];
                int32_t : 8; // Unknown
                int32_t length : 24;
                int32_t : 32;   // Unknown
                int32_t offset; // Relative from current struct offset
            };
            static_assert(sizeof(PartWithoutInfo) == 16);

            struct PartWithInfo
            {
                uint16_t : 16;
                char identifier[2];
                int32_t : 8; // Unknown
                int32_t length : 24;
                int32_t number;
                int32_t offset; // Relative from current struct offset
            };
            static_assert(sizeof(PartWithInfo) == 16);

            struct PartWithIndex
            {
                int16_t index;
                char identifier[2];
                uint32_t : 8; // Unknown
                uint32_t length : 24;
                uint32_t number; // Number of files
                uint32_t offset; // Relative from current struct offset
            };
            static_assert(sizeof(PartWithIndex) == 16);

            struct PartWithInfoOne
            {
                PartInfoOne partInfo;
                char identifier[2];
                int32_t : 8; // Unknown
                int32_t length : 24;
                int32_t number;
                int32_t offset; // Relative from current struct offset
            };
            static_assert(sizeof(PartWithInfoOne) == 16);

            struct PartWithInfoTwo
            {
                PartInfoOne partInfo;
                char identifier[2];
                int32_t : 8; // Unknown
                int32_t length : 24;
                int32_t number;
                int32_t offset; // Relative from current struct offset
            };
            static_assert(sizeof(PartWithInfoTwo) == 16);

            // Possible Part structures:

            using MiscPart = PartWithoutInfo;

            using MaterialPart = PartWithIndex;
            using FSHPart      = PartWithIndex;

            using BasePart = PartWithoutInfo;
            using NamePart = PartWithoutInfo;

            using CullingPart        = PartWithInfoOne;
            using TransformationPart = PartWithInfoOne;
            using VertexPart         = PartWithInfoOne;
            using NormalPart         = PartWithInfoOne;
            using UVPart             = PartWithInfoOne;
            using TrianglePart       = PartWithInfoTwo;
            using EffectPart         = PartWithInfoOne;
        } // namespace CRP
    }     // namespace NFS5
} // namespace LibOpenNFS