#include "NFS2/TRK/TrackBlock.h"
#include "Common/Utils.h"

using namespace LibOpenNFS::NFS2;

template <typename Platform>
void TrackBlock<Platform>::SerializeIn(std::istream &ifstream)
{
    std::streampos trackBlockOffset = ifstream.tellg();

    // Read Header
    Utils::SafeRead(ifstream, blockSize);
    Utils::SafeRead(ifstream, blockSizeDup);
    Utils::SafeRead(ifstream, nExtraBlocks);
    Utils::SafeRead(ifstream, unknown);
    Utils::SafeRead(ifstream, serialNum);
    Utils::SafeRead(ifstream, clippingRect);
    Utils::SafeRead(ifstream, extraBlockTblOffset);
    Utils::SafeRead(ifstream, nStickToNextVerts);
    Utils::SafeRead(ifstream, nLowResVert);
    Utils::SafeRead(ifstream, nMedResVert);
    Utils::SafeRead(ifstream, nHighResVert);
    Utils::SafeRead(ifstream, nLowResPoly);
    Utils::SafeRead(ifstream, nMedResPoly);
    Utils::SafeRead(ifstream, nHighResPoly);
    Utils::SafeRead(ifstream, unknownPad);

    // Sanity Checks
    if (blockSize != blockSizeDup)
        throw std::runtime_error{"Bad Block"};

    // Read 3D Data
    vertexTable.resize(nStickToNextVerts + nHighResVert);
    Utils::SafeRead(ifstream, vertexTable.begin(), vertexTable.end());

    polygonTable.resize(nLowResPoly + nMedResPoly + nHighResPoly);
    Utils::SafeRead(ifstream, polygonTable.begin(), polygonTable.end());

    // Read Extrablock data
    ifstream.seekg((uint32_t) trackBlockOffset + 64u + extraBlockTblOffset, std::ios_base::beg);
    // Get extrablock offsets (relative to beginning of TrackBlock)
    extraBlockOffsets.resize(nExtraBlocks);
    Utils::SafeRead(ifstream, extraBlockOffsets.begin(), extraBlockOffsets.end());

    for (uint32_t extraBlockIdx = 0; extraBlockIdx < nExtraBlocks; ++extraBlockIdx)
    {
        ifstream.seekg((uint32_t) trackBlockOffset + extraBlockOffsets[extraBlockIdx], std::ios_base::beg);

        ExtraObjectBlock<Platform> extraObjectBlock;
        ifstream >> extraObjectBlock;
        extraObjectBlocks.push_back(std::move(extraObjectBlock));
        // Map the the block type to the vector index, original ordering is then maintained for output serialisation
        extraObjectBlockMap[(ExtraBlockID) extraObjectBlocks.back().id] = extraBlockIdx;
    }
}

template <typename Platform>
ExtraObjectBlock<Platform> TrackBlock<Platform>::GetExtraObjectBlock(ExtraBlockID eBlockType)
{
    return extraObjectBlocks[extraObjectBlockMap[eBlockType]];
}

template <typename Platform>
bool TrackBlock<Platform>::IsBlockPresent(ExtraBlockID eBlockType)
{
    return extraObjectBlockMap.count(eBlockType);
}

template class LibOpenNFS::NFS2::TrackBlock<PS1>;
template class LibOpenNFS::NFS2::TrackBlock<PC>;
