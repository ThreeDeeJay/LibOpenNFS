#include <cstring>

#include "NFS3/COL/ColFile.h"

using namespace LibOpenNFS::NFS3;

void ColFile::SerializeIn(std::istream &ifstream)
{
    Utils::SafeRead(ifstream, header);
    Utils::SafeRead(ifstream, version);
    Utils::SafeRead(ifstream, fileLength);
    Utils::SafeRead(ifstream, nBlocks);

    if ((memcmp(header, "COLL", sizeof(char)) != 0) || (version != 11) || ((nBlocks != 2) && (nBlocks != 4) && (nBlocks != 5)))
        throw std::runtime_error{"Invalid COL file"};

    Utils::SafeRead(ifstream, xbTable, sizeof(uint32_t) * nBlocks);

    // texture XB
    Utils::SafeRead(ifstream, textureHead);
    if (textureHead.xbid != XBID::TextureInfo)
        throw;

    texture.resize(textureHead.nrec);
    Utils::SafeRead(ifstream, texture.begin(), texture.end());

    // struct3D XB
    if (nBlocks >= 4)
    {
        Utils::SafeRead(ifstream, struct3DHead);
        if (struct3DHead.xbid != XBID::Struct3D)
            throw;

        struct3D.resize(struct3DHead.nrec);
        for (uint32_t colRec_Idx = 0; colRec_Idx < struct3DHead.nrec; colRec_Idx++)
        {
            Utils::SafeRead(ifstream, struct3D[colRec_Idx].size);
            Utils::SafeRead(ifstream, struct3D[colRec_Idx].nVert);
            Utils::SafeRead(ifstream, struct3D[colRec_Idx].nPoly);

            int32_t delta = (8 + sizeof(ColVertex) * struct3D[colRec_Idx].nVert + sizeof(ColPolygon) * struct3D[colRec_Idx].nPoly) % 4;
            delta         = (4 - delta) % 4;

            // Check the size matches up with the expected size of the contents
            if (struct3D[colRec_Idx].size != 8 + sizeof(ColVertex) * struct3D[colRec_Idx].nVert + sizeof(ColPolygon) * struct3D[colRec_Idx].nPoly + delta)
                throw;

            // Grab the vertices
            struct3D[colRec_Idx].vertex.resize(struct3D[colRec_Idx].nVert);
            Utils::SafeRead(ifstream, struct3D[colRec_Idx].vertex.begin(), struct3D[colRec_Idx].vertex.end());

            // And Polygons
            struct3D[colRec_Idx].polygon.resize(struct3D[colRec_Idx].nPoly);
            Utils::SafeRead(ifstream, struct3D[colRec_Idx].polygon.begin(), struct3D[colRec_Idx].polygon.end());

            // Consume the delta, to eat alignment bytes
            int dummy;
            if (delta > 0)
                Utils::SafeRead(ifstream, &dummy, delta);
        }

        // TODO: Share this code between both XOBJ parse runs
        // object XB
        Utils::SafeRead(ifstream, objectHead);
        if ((objectHead.xbid != XBID::Object) && (objectHead.xbid != XBID::Object2))
            throw;

        object.resize(objectHead.nrec);
        for (uint32_t xobjIdx = 0; xobjIdx < objectHead.nrec; xobjIdx++)
        {
            Utils::SafeRead(ifstream, object[xobjIdx].size);
            Utils::SafeRead(ifstream, object[xobjIdx].type);
            Utils::SafeRead(ifstream, object[xobjIdx].struct3D);

            if (object[xobjIdx].type == 1)
            {
                if (object[xobjIdx].size != 16)
                    throw;

                Utils::SafeRead(ifstream, object[xobjIdx].ptRef);
            }
            else if (object[xobjIdx].type == 3)
            {
                Utils::SafeRead(ifstream, object[xobjIdx].animLength);
                Utils::SafeRead(ifstream, object[xobjIdx].unknown);
                if (object[xobjIdx].size != 8 + 20 * object[xobjIdx].animLength)
                    throw;

                object[xobjIdx].animData.resize(object[xobjIdx].animLength);
                Utils::SafeRead(ifstream, object[xobjIdx].animData.begin(), object[xobjIdx].animData.end());
                // Make a ref point from first anim position
                object[xobjIdx].ptRef = Utils::FixedToFloat(object[xobjIdx].animData[0].pt);
            }
            else
            {
                // Unknown object type
                throw;
            }
        }
    }

    // object2 XB
    if (nBlocks == 5)
    {
        Utils::SafeRead(ifstream, object2Head);
        if ((object2Head.xbid != XBID::Object) && (object2Head.xbid != XBID::Object2))
            throw;

        object2.resize(object2Head.nrec);
        for (uint32_t xobjIdx = 0; xobjIdx < object2Head.nrec; xobjIdx++)
        {
            Utils::SafeRead(ifstream, object2[xobjIdx].size);
            Utils::SafeRead(ifstream, object2[xobjIdx].type);
            Utils::SafeRead(ifstream, object2[xobjIdx].struct3D);

            if (object2[xobjIdx].type == 1)
            {
                if (object2[xobjIdx].size != 16)
                    throw;

                Utils::SafeRead(ifstream, object2[xobjIdx].ptRef);
            }
            else if (object2[xobjIdx].type == 3)
            {
                Utils::SafeRead(ifstream, object2[xobjIdx].animLength);
                Utils::SafeRead(ifstream, object2[xobjIdx].unknown);
                if (object2[xobjIdx].size != 8 + 20 * object2[xobjIdx].animLength)
                    throw;

                object2[xobjIdx].animData.resize(object2[xobjIdx].animLength);
                Utils::SafeRead(ifstream, object2[xobjIdx].animData.begin(), object2[xobjIdx].animData.end());
                // Make a ref point from first anim position
                object2[xobjIdx].ptRef = Utils::FixedToFloat(object2[xobjIdx].animData[0].pt);
            }
            else
            {
                // Unknown object2 type
                throw;
            }
        }
    }

    // vroad XB
    Utils::SafeRead(ifstream, vroadHead);
    if (vroadHead.xbid != XBID::VRoad || (vroadHead.size != 8 + sizeof(ColVRoad) * vroadHead.nrec))
        throw;

    vroad.resize(vroadHead.nrec);
    Utils::SafeRead(ifstream, vroad.begin(), vroad.end());
}
