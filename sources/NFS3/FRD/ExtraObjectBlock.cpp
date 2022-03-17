#include "NFS3/FRD/ExtraObjectBlock.h"

using namespace LibOpenNFS::NFS3;

void ExtraObjectBlock::SerializeIn(std::istream &ifstream)
{
    Utils::SafeRead(ifstream, nobj);
    obj.reserve(nobj);

    for (uint32_t xobjIdx = 0; xobjIdx < nobj; ++xobjIdx)
    {
        ExtraObjectData x;

        Utils::SafeRead(ifstream, x.crosstype);
        Utils::SafeRead(ifstream, x.crossno);
        Utils::SafeRead(ifstream, x.unknown);

        if (x.crosstype == 4)
        {
            // Basic objects
            Utils::SafeRead(ifstream, x.ptRef);
            Utils::SafeRead(ifstream, x.AnimMemory);
        }
        else if (x.crosstype == 3)
        {
            // Animated objects
            Utils::SafeRead(ifstream, x.unknown3);
            Utils::SafeRead(ifstream, x.type3);
            Utils::SafeRead(ifstream, x.objno);
            Utils::SafeRead(ifstream, x.nAnimLength);
            Utils::SafeRead(ifstream, x.AnimDelay);

            // Sanity Check
            if (x.type3 != 3)
                throw;
            
            x.animData.resize(x.nAnimLength);
            Utils::SafeRead(ifstream, x.animData.begin(), x.animData.end());

            // make a ref point from first anim position
            x.ptRef = Utils::FixedToFloat(x.animData[0].pt);
        }
        else
            throw; // unknown object type

        // Get number of vertices
        Utils::SafeRead(ifstream, x.nVertices);

        // Get vertices
        x.vert.resize(x.nVertices);
        Utils::SafeRead(ifstream, x.vert.begin(), x.vert.end());

        // Per vertex shading data (RGBA)
        x.vertShading.resize(x.nVertices);
        Utils::SafeRead(ifstream, x.vertShading.begin(), x.vertShading.end());

        // Get number of polygons
        Utils::SafeRead(ifstream, x.nPolygons);

        // Grab the polygons
        x.polyData.resize(x.nPolygons);
        Utils::SafeRead(ifstream, x.polyData.begin(), x.polyData.end());

        obj.push_back(x);
    }
}

void ExtraObjectBlock::SerializeOut(std::ostream &ofstream)
{
    ofstream.write((char *) &(nobj), sizeof(uint32_t));

    for (uint32_t xobjIdx = 0; xobjIdx < nobj; ++xobjIdx)
    {
        ofstream.write((char *) &obj[xobjIdx].crosstype, sizeof(uint32_t));
        ofstream.write((char *) &obj[xobjIdx].crossno, sizeof(uint32_t));
        ofstream.write((char *) &obj[xobjIdx].unknown, sizeof(uint32_t));

        if (obj[xobjIdx].crosstype == 4)
        {
            // Basic objects
            ofstream.write((char *) &obj[xobjIdx].ptRef, sizeof(glm::vec3));
            ofstream.write((char *) &obj[xobjIdx].AnimMemory, sizeof(uint32_t));
        }
        else if (obj[xobjIdx].crosstype == 3)
        {
            // Animated objects
            ofstream.write((char *) &obj[xobjIdx].unknown3, sizeof(uint16_t) * 9);
            ofstream.write((char *) &obj[xobjIdx].type3, sizeof(uint8_t));
            ofstream.write((char *) &obj[xobjIdx].objno, sizeof(uint8_t));
            ofstream.write((char *) &obj[xobjIdx].nAnimLength, sizeof(uint16_t));
            ofstream.write((char *) &obj[xobjIdx].AnimDelay, sizeof(uint16_t));
            ofstream.write((char *) obj[xobjIdx].animData.data(), sizeof(AnimData) * obj[xobjIdx].nAnimLength);
        }
        ofstream.write((char *) &(obj[xobjIdx].nVertices), sizeof(uint32_t));
        ofstream.write((char *) obj[xobjIdx].vert.data(), sizeof(glm::vec3) * obj[xobjIdx].nVertices);
        ofstream.write((char *) obj[xobjIdx].vertShading.data(), sizeof(uint32_t) * obj[xobjIdx].nVertices);
        ofstream.write((char *) &(obj[xobjIdx].nPolygons), sizeof(uint32_t));
        ofstream.write((char *) obj[xobjIdx].polyData.data(), sizeof(PolygonData) * obj[xobjIdx].nPolygons);
    }
}