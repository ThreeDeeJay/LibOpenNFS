#pragma once

#include <cstdint>

namespace LibOpenNFS
{
    namespace NFS5
    {
        namespace CRP
        {
            struct ColorBGRA
            {
                uint8_t Blue;
                uint8_t Green;
                uint8_t Red;
                uint8_t Alpha;
            };

            struct Vector2f
            {
                float x, y;
            };
            static_assert(sizeof(Vector2f) == 8);

            struct Vector3f
            {
                float x, y, z;
            };
            static_assert(sizeof(Vector3f) == 12);

            struct Vector4f
            {
                float x, y, z, w;
            };
            static_assert(sizeof(Vector4f) == 16);

            using Matrix4f = float[16];
            static_assert(sizeof(Matrix4f) == 64);

        } // namespace CRP
    }     // namespace NFS5
} // namespace LibOpenNFS