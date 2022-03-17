#include "NFS2/TRK/SuperBlock.h"
#include "Common/Utils.h"

using namespace LibOpenNFS::NFS2;

template <typename Platform>
void SuperBlock<Platform>::SerializeIn(std::istream &ifstream)
{
    // TODO: Gross, needs to be relative//passed in
    std::streampos superblockOffset = ifstream.tellg();
    Utils::SafeRead(ifstream, superBlockSize);
    Utils::SafeRead(ifstream, nBlocks);
    Utils::SafeRead(ifstream, padding);

    if (nBlocks != 0)
    {
        // Get the offsets of the child blocks within superblock
        blockOffsets.resize(nBlocks);
        Utils::SafeRead(ifstream, blockOffsets.begin(), blockOffsets.end());

        for (uint32_t blockIdx = 0; blockIdx < nBlocks; ++blockIdx)
        {
            // LOG(DEBUG) << "  Block " << block_Idx + 1 << " of " << superblock->nBlocks << " [" << trackblock->header->serialNum << "]";
            // TODO: Fix this
            ifstream.seekg((uint32_t) superblockOffset + blockOffsets[blockIdx], std::ios_base::beg);

            TrackBlock<Platform> trackBlock;
            ifstream >> trackBlock;
            trackBlocks.push_back(std::move(trackBlock));
        }
    }
}

template class LibOpenNFS::NFS2::SuperBlock<PS1>;
template class LibOpenNFS::NFS2::SuperBlock<PC>;