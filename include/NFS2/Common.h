#pragma once

#include <cstdint>
#include <array>
#include <string_view>
#include <string>

#include <glm/glm.hpp>

namespace LibOpenNFS
{
    namespace NFS2
    {
        enum class Platform
        {
            PC,
            PS1
        };

        struct VertexHighP
        {
            int32_t x, z, y;
        };

        struct AnimPosition
        {
            VertexHighP position;
            int16_t unknown[4];
        };

        template <Platform>
        struct Vertex;

        template <>
        struct Vertex<Platform::PC>
        {
            int16_t x, z, y;
        };

        template <>
        struct Vertex<Platform::PS1>
        {
            int16_t x, z, y, w;
        };

        template <Platform>
        struct PolygonData;

        template <>
        struct PolygonData<Platform::PC>
        {
            int16_t texture;
            int16_t otherSideTex;
            uint8_t vertex[4];
        };

        template <>
        struct PolygonData<Platform::PS1>
        {
            uint8_t texture;
            uint8_t otherSideTex;
            uint8_t vertex[4];
        };
    } // namespace NFS2
} // namespace LibOpenNFS
