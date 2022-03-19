#include "NFS2/TRK/SuperBlock.h"

using namespace LibOpenNFS::NFS2;

template <Platform platform>
void SuperBlock<platform>::SerializeIn(std::istream &is)
{
    // TODO: Gross, needs to be relative//passed in
    auto superblockOffset = is.tellg();
    Utils::SafeRead(is, superBlockSize);
    Utils::SafeRead(is, nBlocks);
    Utils::SafeRead(is, padding);

    if (nBlocks != 0)
    {
        // Get the offsets of the child blocks within superblock
        blockOffsets.resize(nBlocks);
        Utils::SafeRead(is, blockOffsets.begin(), blockOffsets.end());

        for (uint32_t blockIdx = 0; blockIdx < nBlocks; ++blockIdx)
        {
            // LOG(DEBUG) << "  Block " << block_Idx + 1 << " of " << superblock->nBlocks << " [" << trackblock->header->serialNum << "]";
            // TODO: Fix this
            is.seekg(superblockOffset + blockOffsets[blockIdx], std::ios_base::beg);

            is >> trackBlocks.emplace_back();
        }
    }
}

template class LibOpenNFS::NFS2::SuperBlock<Platform::PS1>;
template class LibOpenNFS::NFS2::SuperBlock<Platform::PC>;