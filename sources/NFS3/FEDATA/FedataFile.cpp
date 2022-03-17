#include "NFS3/FEDATA/FedataFile.h"

using namespace LibOpenNFS::NFS3;

void FedataFile::SerializeIn(std::istream &ifstream)
{
    // TODO: Hugely incomplete. Old style parser shoehorned into new format, need all structs. No seekg. /AS
    // Go get the offset of car name
    uint32_t menuNameOffset = 0;
    ifstream.seekg(cMenuNameFilePosOffset, std::ios::beg);
    Utils::SafeRead(ifstream, menuNameOffset);
    ifstream.seekg(menuNameOffset, std::ios::beg);

    char carMenuName[64];
    Utils::SafeRead(ifstream, carMenuName);
    menuName = carMenuName;

    // Jump to location of FILEPOS table for car colour names
    ifstream.seekg(cColourTableOffset, std::ios::beg);
    // Read that table in
    std::vector<uint32_t> colourNameOffsets(m_nPriColours);
    Utils::SafeRead(ifstream, colourNameOffsets.begin(), colourNameOffsets.end());

    for (uint8_t colourIdx = 0; colourIdx < m_nPriColours; ++colourIdx)
    {
        ifstream.seekg(colourNameOffsets[colourIdx], std::ios::beg);
        std::string colourName;
        std::getline(ifstream, colourName, '\0');
        primaryColourNames.emplace_back(colourName.begin(), colourName.end());
    }
}