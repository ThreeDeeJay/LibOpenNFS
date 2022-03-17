#pragma once

#include <cstdint>
#include <vector>

#include "Common/Utils.h"
#include "NFS2/Common.h"

namespace LibOpenNFS
{
    namespace NFS2
    {
        class StructureRefBlock : public IDeserializable
        {
        public:
            // XBID = 7, 18
            uint16_t recSize; // If Anim (recType == 3) , recSize == 8 + 20*animation length
            uint8_t recType;
            uint8_t structureRef;
            // Fixed Type (recType == 1)
            VertexHighP refCoordinates;
            // Animated Type (recType == 3)
            uint16_t animLength;                 // num of position records
            uint16_t unknown;                    // Potentially time between animation steps?
            std::vector<AnimPosition> animationData; // Sequence of positions which animation follows

        private:
            void SerializeIn(std::istream &ifstream) override;
        };
    } // namespace NFS2
} // namespace LibOpenNFS
