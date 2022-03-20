#pragma once

#include <vector>

#include "NFS5/CRP/Types.hpp"
#include "NFS5/CRP/Parts.hpp"

#include "Common/Utils.h"

namespace LibOpenNFS
{
    namespace NFS5
    {
        class CRPFile : public IDeserializable
        {
        public:
            // Modified Arushan CRP decompressor. Removes LZ77 style decompression from CRPs
            static std::vector<char> Decompress(std::istream& compressed);
            static bool IsCompressed(std::istream& is);

            CRP::Header header;
            std::vector<CRP::Article> articles;
            std::vector<CRP::MiscPart> miscParts;

        protected:
            void SerializeIn(std::istream& ifstream) override;
        };
    } // namespace NFS5
} // namespace LibOpenNFS
