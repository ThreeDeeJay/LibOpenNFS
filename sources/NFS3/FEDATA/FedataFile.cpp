#include "NFS3/FEDATA/FedataFile.h"

using namespace LibOpenNFS::NFS3;

void FedataFile::SerializeIn(std::istream &is)
{
    // TODO: Hugely incomplete. Old style parser shoehorned into new format, need all structs. No seekg. /AS
    // Go get the offset of car name
    uint32_t menuNameOffset = 0;
    is.seekg(cMenuNameFilePosOffset, std::ios::beg);
    Utils::SafeRead(is, menuNameOffset);
    
    menuName.resize(64);
    is.seekg(menuNameOffset, std::ios::beg);
    Utils::SafeRead(is, menuName.begin(), menuName.end());

    // Jump to location of FILEPOS table for car colour names
    is.seekg(cColourTableOffset, std::ios::beg);

    // Read that table in
    std::vector<uint32_t> colourNameOffsets(m_nPriColours);
    Utils::SafeRead(is, colourNameOffsets.begin(), colourNameOffsets.end());

    for (uint8_t colourIdx = 0; colourIdx < m_nPriColours; ++colourIdx)
    {
        is.seekg(colourNameOffsets[colourIdx], std::ios::beg);
        std::getline(is, primaryColourNames.emplace_back(), '\0');
    }
}