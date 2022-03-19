#include "NFS2/COL/ColFile.h"

#include <cstring>
#include <cassert>

using namespace LibOpenNFS;
using namespace LibOpenNFS::NFS2;

template <Platform platform>
void ColFile<platform>::SerializeIn(std::istream &is)
{
    auto beginSeek = is.tellg();

    // Check we're in a valid TRK file
    Utils::SafeRead(is, header);

    if (strncmp(header, "COLL", sizeof(header)) != 0)
        throw std::runtime_error{"Invalid Col Header(s)"};

    Utils::SafeRead(is, colVersion);
    if (colVersion != 11)
        throw std::runtime_error{"Invalid Col version"};

    Utils::SafeRead(is, size);
    Utils::SafeRead(is, nExtraBlocks);

    extraBlockOffsets.resize(nExtraBlocks);
    Utils::SafeRead(is, extraBlockOffsets.begin(), extraBlockOffsets.end());

    for (uint32_t extraBlockIdx = 0; extraBlockIdx < nExtraBlocks; ++extraBlockIdx)
    {
        is.seekg(beginSeek + 16 + extraBlockOffsets[extraBlockIdx], std::ios_base::beg);
        
        is >> extraObjectBlocks.emplace_back();

        // Map the the block type to the vector index, gross, original ordering is then maintained for output serialisation
        extraObjectBlockMap[static_cast<ExtraBlockID>(extraObjectBlocks.back().id)] = extraBlockIdx;
    }
}

template <Platform platform>
ExtraObjectBlock<platform> ColFile<platform>::GetExtraObjectBlock(ExtraBlockID eBlockType)
{
    return extraObjectBlocks[extraObjectBlockMap[eBlockType]];
}

template <Platform platform>
bool ColFile<platform>::IsBlockPresent(ExtraBlockID eBlockType)
{
    return extraObjectBlockMap.count(eBlockType);
}

template class LibOpenNFS::NFS2::ColFile<Platform::PS1>;
template class LibOpenNFS::NFS2::ColFile<Platform::PC>;