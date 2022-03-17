#include "NFS3/FRD/TexBlock.h"

using namespace LibOpenNFS::NFS3;

void TexBlock::SerializeIn(std::istream &ifstream)
{
    Utils::SafeRead(ifstream, width);
    Utils::SafeRead(ifstream, height);
    Utils::SafeRead(ifstream, unknown1);
    Utils::SafeRead(ifstream, corners);
    Utils::SafeRead(ifstream, unknown2);
    Utils::SafeRead(ifstream, isLane);
    Utils::SafeRead(ifstream, qfsIndex);
}

void TexBlock::SerializeOut(std::ostream &ofstream) const
{
    // TODO: Do I need to align here?
    ofstream.write((char *) &width, sizeof(uint16_t));
    ofstream.write((char *) &height, sizeof(uint16_t));
    ofstream.write((char *) &unknown1, sizeof(uint32_t));
    ofstream.write((char *) &corners, sizeof(float) * 8);
    ofstream.write((char *) &unknown2, sizeof(uint32_t));
    ofstream.write((char *) &isLane, sizeof(bool));
    ofstream.write((char *) &qfsIndex, sizeof(uint16_t));
}