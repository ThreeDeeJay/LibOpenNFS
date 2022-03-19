#include "NFS2/TRK/TrackBlock.h"

using namespace LibOpenNFS::NFS2;

template <Platform platform>
void TrackBlock<platform>::SerializeIn(std::istream &is)
{
    auto trackBlockOffset{is.tellg()};

    // Read Header
    Utils::SafeRead(is, blockSize);
    Utils::SafeRead(is, blockSizeDup);
    Utils::SafeRead(is, nExtraBlocks);
    Utils::SafeRead(is, unknown);
    Utils::SafeRead(is, serialNum);
    Utils::SafeRead(is, clippingRect);
    Utils::SafeRead(is, extraBlockTblOffset);
    Utils::SafeRead(is, nStickToNextVerts);
    Utils::SafeRead(is, nLowResVert);
    Utils::SafeRead(is, nMedResVert);
    Utils::SafeRead(is, nHighResVert);
    Utils::SafeRead(is, nLowResPoly);
    Utils::SafeRead(is, nMedResPoly);
    Utils::SafeRead(is, nHighResPoly);
    Utils::SafeRead(is, unknownPad);

    // Sanity Checks
    if (blockSize != blockSizeDup)
        throw std::runtime_error{"Bad TrackBlock"};

    // Read 3D Data
    vertexTable.resize(nStickToNextVerts + nHighResVert);
    Utils::SafeRead(is, vertexTable.begin(), vertexTable.end());

    polygonTable.resize(nLowResPoly + nMedResPoly + nHighResPoly);
    Utils::SafeRead(is, polygonTable.begin(), polygonTable.end());

    // Read Extrablock data
    is.seekg(trackBlockOffset + 64u + extraBlockTblOffset, std::ios_base::beg);

    // Get extrablock offsets (relative to beginning of TrackBlock)
    extraBlockOffsets.resize(nExtraBlocks);
    Utils::SafeRead(is, extraBlockOffsets.begin(), extraBlockOffsets.end());

    for (uint32_t extraBlockIdx = 0; extraBlockIdx < nExtraBlocks; ++extraBlockIdx)
    {
        is.seekg(trackBlockOffset + extraBlockOffsets[extraBlockIdx], std::ios_base::beg);
        is >> extraObjectBlocks.emplace_back();
        
        // Map the the block type to the vector index, original ordering is then maintained for output serialisation
        extraObjectBlockMap[(ExtraBlockID) extraObjectBlocks.back().id] = extraBlockIdx;
    }
}

template <Platform platform>
ExtraObjectBlock<platform> TrackBlock<platform>::GetExtraObjectBlock(ExtraBlockID eBlockType)
{
    return extraObjectBlocks[extraObjectBlockMap[eBlockType]];
}

template <Platform platform>
bool TrackBlock<platform>::IsBlockPresent(ExtraBlockID eBlockType)
{
    return extraObjectBlockMap.count(eBlockType);
}

template class LibOpenNFS::NFS2::TrackBlock<Platform::PS1>;
template class LibOpenNFS::NFS2::TrackBlock<Platform::PC>;
