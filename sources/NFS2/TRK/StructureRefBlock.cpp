#include "NFS2/TRK/StructureRefBlock.h"

using namespace LibOpenNFS::NFS2;

void StructureRefBlock::SerializeIn(std::istream &is)
{
    auto padCheck = is.tellg();

    Utils::SafeRead(is, recSize);
    Utils::SafeRead(is, recType);
    Utils::SafeRead(is, structureRef);

    if (recType == 1)
    {
        // Fixed type
        Utils::SafeRead(is, refCoordinates);
    }
    else if (recType == 3)
    {
        // Animated type
        Utils::SafeRead(is, animLength);
        Utils::SafeRead(is, unknown);
        animationData.resize(animLength);
        Utils::SafeRead(is, animationData.begin(), animationData.end());
    }
    else if (recType == 4)
    {
        // 4 Component PSX Vert data? TODO: Restructure to allow the 4th component to be read
        Utils::SafeRead(is, refCoordinates);
    }
    else
    {
        // LOG(DEBUG) << "Unknown Structure Reference type: " << (int) recType << " Size: " << (int) recSize << " StructRef: " << (int) structureRef;
        throw std::runtime_error{"Unknown Structure Reference type"};
    }

    is.seekg(recSize - (is.tellg() - padCheck), std::ios_base::cur); // Eat possible padding
}