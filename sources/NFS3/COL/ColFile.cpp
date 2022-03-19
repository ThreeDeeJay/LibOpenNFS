#include <cstring>

#include "NFS3/COL/ColFile.h"

using namespace LibOpenNFS::NFS3;

void ColFile::SerializeIn(std::istream &is)
{
    Utils::SafeRead(is, header);
    Utils::SafeRead(is, version);
    Utils::SafeRead(is, fileLength);
    Utils::SafeRead(is, nBlocks);

    if ((memcmp(header, "COLL", sizeof(char)) != 0) || (version != 11) || ((nBlocks != 2) && (nBlocks != 4) && (nBlocks != 5)))
        throw std::runtime_error{"Invalid COL file"};

    Utils::SafeRead(is, xbTable, sizeof(uint32_t) * nBlocks);

    // texture XB
    Utils::SafeRead(is, textureHead);
    if (textureHead.xbid != XBID::TextureInfo)
        throw;

    texture.resize(textureHead.nrec);
    Utils::SafeRead(is, texture.begin(), texture.end());

    // struct3D XB
    if (nBlocks >= 4)
    {
        Utils::SafeRead(is, struct3DHead);
        if (struct3DHead.xbid != XBID::Struct3D)
            throw;

        struct3D.resize(struct3DHead.nrec);
        for (uint32_t colRec_Idx = 0; colRec_Idx < struct3DHead.nrec; colRec_Idx++)
        {
            Utils::SafeRead(is, struct3D[colRec_Idx].size);
            Utils::SafeRead(is, struct3D[colRec_Idx].nVert);
            Utils::SafeRead(is, struct3D[colRec_Idx].nPoly);

            int32_t delta = (8 + sizeof(ColVertex) * struct3D[colRec_Idx].nVert + sizeof(ColPolygon) * struct3D[colRec_Idx].nPoly) % 4;
            delta         = (4 - delta) % 4;

            // Check the size matches up with the expected size of the contents
            if (struct3D[colRec_Idx].size != 8 + sizeof(ColVertex) * struct3D[colRec_Idx].nVert + sizeof(ColPolygon) * struct3D[colRec_Idx].nPoly + delta)
                throw;

            // Grab the vertices
            struct3D[colRec_Idx].vertex.resize(struct3D[colRec_Idx].nVert);
            Utils::SafeRead(is, struct3D[colRec_Idx].vertex.begin(), struct3D[colRec_Idx].vertex.end());

            // And Polygons
            struct3D[colRec_Idx].polygon.resize(struct3D[colRec_Idx].nPoly);
            Utils::SafeRead(is, struct3D[colRec_Idx].polygon.begin(), struct3D[colRec_Idx].polygon.end());

            // Consume the delta, to eat alignment bytes
            int dummy;
            if (delta > 0)
                Utils::SafeRead(is, &dummy, delta);
        }

        // TODO: Share this code between both XOBJ parse runs
        // object XB
        Utils::SafeRead(is, objectHead);
        if ((objectHead.xbid != XBID::Object) && (objectHead.xbid != XBID::Object2))
            throw;

        object.resize(objectHead.nrec);
        for (uint32_t xobjIdx = 0; xobjIdx < objectHead.nrec; xobjIdx++)
        {
            Utils::SafeRead(is, object[xobjIdx].size);
            Utils::SafeRead(is, object[xobjIdx].type);
            Utils::SafeRead(is, object[xobjIdx].struct3D);

            if (object[xobjIdx].type == 1)
            {
                if (object[xobjIdx].size != 16)
                    throw;

                Utils::SafeRead(is, object[xobjIdx].ptRef);
            }
            else if (object[xobjIdx].type == 3)
            {
                Utils::SafeRead(is, object[xobjIdx].animLength);
                Utils::SafeRead(is, object[xobjIdx].unknown);
                if (object[xobjIdx].size != 8 + 20 * object[xobjIdx].animLength)
                    throw;

                object[xobjIdx].animData.resize(object[xobjIdx].animLength);
                Utils::SafeRead(is, object[xobjIdx].animData.begin(), object[xobjIdx].animData.end());
                // Make a ref point from first anim position
                object[xobjIdx].ptRef = Utils::FixedToFloat(object[xobjIdx].animData[0].pt);
            }
            else
            {
                // Unknown object type
                throw std::runtime_error{"Unknown object type"};
            }
        }
    }

    // object2 XB
    if (nBlocks == 5)
    {
        Utils::SafeRead(is, object2Head);
        if ((object2Head.xbid != XBID::Object) && (object2Head.xbid != XBID::Object2))
            throw;

        object2.resize(object2Head.nrec);
        for (uint32_t xobjIdx = 0; xobjIdx < object2Head.nrec; xobjIdx++)
        {
            Utils::SafeRead(is, object2[xobjIdx].size);
            Utils::SafeRead(is, object2[xobjIdx].type);
            Utils::SafeRead(is, object2[xobjIdx].struct3D);

            if (object2[xobjIdx].type == 1)
            {
                if (object2[xobjIdx].size != 16)
                    throw;

                Utils::SafeRead(is, object2[xobjIdx].ptRef);
            }
            else if (object2[xobjIdx].type == 3)
            {
                Utils::SafeRead(is, object2[xobjIdx].animLength);
                Utils::SafeRead(is, object2[xobjIdx].unknown);
                if (object2[xobjIdx].size != 8 + 20 * object2[xobjIdx].animLength)
                    throw;

                object2[xobjIdx].animData.resize(object2[xobjIdx].animLength);
                Utils::SafeRead(is, object2[xobjIdx].animData.begin(), object2[xobjIdx].animData.end());
                // Make a ref point from first anim position
                object2[xobjIdx].ptRef = Utils::FixedToFloat(object2[xobjIdx].animData[0].pt);
            }
            else
            {
                // Unknown object2 type
                throw std::runtime_error{"Unknown object2 type"};
            }
        }
    }

    // vroad XB
    Utils::SafeRead(is, vroadHead);
    if (vroadHead.xbid != XBID::VRoad || (vroadHead.size != 8 + sizeof(ColVRoad) * vroadHead.nrec))
        throw;

    vroad.resize(vroadHead.nrec);
    Utils::SafeRead(is, vroad.begin(), vroad.end());
}
