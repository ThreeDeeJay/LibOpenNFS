#include "NFS2/COL/ColFile.h"
#include "Common/Utils.h"

#include <cstring>
#include <cassert>

using namespace LibOpenNFS;
using namespace LibOpenNFS::NFS2;

template <typename Platform>
void ColFile<Platform>::SerializeIn(std::istream &ifstream)
{
    // Check we're in a valid TRK file
    Utils::SafeRead(ifstream, header);

    if(strncmp(header, "COLL", sizeof(header)) != 0)
        throw;

    Utils::SafeRead(ifstream, colVersion);
    if (colVersion != 11)
        throw;

    Utils::SafeRead(ifstream, size);
    Utils::SafeRead(ifstream, nExtraBlocks);

    extraBlockOffsets.resize(nExtraBlocks);
    Utils::SafeRead(ifstream, extraBlockOffsets.begin(), extraBlockOffsets.end());

    for (uint32_t extraBlockIdx = 0; extraBlockIdx < nExtraBlocks; ++extraBlockIdx)
    {
        ifstream.seekg(16 + extraBlockOffsets[extraBlockIdx], std::ios_base::beg);
        
        ExtraObjectBlock<Platform> extraObjectBlock;
        ifstream >> extraObjectBlock;
        
        extraObjectBlocks.push_back(std::move(extraObjectBlock));

        // Map the the block type to the vector index, gross, original ordering is then maintained for output serialisation
        extraObjectBlockMap[static_cast<ExtraBlockID>(extraObjectBlocks.back().id)] = extraBlockIdx;
    }
}

template <typename Platform>
ExtraObjectBlock<Platform> ColFile<Platform>::GetExtraObjectBlock(ExtraBlockID eBlockType)
{
    return extraObjectBlocks[extraObjectBlockMap[eBlockType]];
}

template <typename Platform>
bool ColFile<Platform>::IsBlockPresent(ExtraBlockID eBlockType)
{
    return extraObjectBlockMap.count(eBlockType);
}

template class LibOpenNFS::NFS2::ColFile<PS1>;
template class LibOpenNFS::NFS2::ColFile<PC>;