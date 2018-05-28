//
// Created by Amrik on 24/05/2018.
//

// TODO: Base class the track loader and extend for each NFS, so dont have to carry this shitty class name around
#include <iostream>
#include "nfs2_trk_loader.h"

using namespace std;

nfs2_trk_loader::nfs2_trk_loader(const std::string &trk_path) {
    std::cout << "Loading NFS2 Track.. " << std::endl;
    LoadTRK(trk_path);
    // TODO: Load Catalogue file to get global (non trkblock specific) data
    //LoadCOL(col_path);
}

bool nfs2_trk_loader::LoadTRK(std::string trk_path) {
    ifstream ar(trk_path, ios::in | ios::binary);
    // Check we're in a valid TRK file
    unsigned char header[4];
    if (ar.read(((char *) header), sizeof(unsigned char) * 4).gcount() != sizeof(unsigned char) * 4) return false;
    if (!strcmp(reinterpret_cast<const char *>(header), "TRAC")) return false;

    // Unknown header data
    if (ar.read(((char *) unknownHeader), sizeof(uint32_t) * 5).gcount() != sizeof(uint32_t) * 5) return false;

    // Basic Track data
    ar.read((char *) &nSuperBlocks, sizeof(uint32_t));
    ar.read((char *) &nBlocks, sizeof(uint32_t));
    superblocks = (NFS2_SUPERBLOCK *) calloc(static_cast<size_t>(nBlocks), sizeof(NFS2_SUPERBLOCK));

    // Offsets of Superblocks in TRK file
    uint32_t superblockOffsets[nSuperBlocks];
    if (ar.read(((char *) superblockOffsets), nSuperBlocks * sizeof(uint32_t)).gcount() !=
        nSuperBlocks * sizeof(uint32_t))
        return false;

    // Reference coordinates for each block
    NFS2_VERT_HIGH *blockReferenceCoords = (NFS2_VERT_HIGH *) calloc(static_cast<size_t>(nBlocks), sizeof(NFS2_VERT_HIGH));
    if (ar.read(((char *) blockReferenceCoords), nBlocks * sizeof(NFS2_VERT_HIGH)).gcount() != nBlocks * sizeof(NFS2_VERT_HIGH)){
        free(blockReferenceCoords);
        return false;
    }

    for (int superBlock_Idx = 0; superBlock_Idx < nSuperBlocks; ++superBlock_Idx) {
        std::cout << "SuperBlock " << superBlock_Idx+1 << " of " << nSuperBlocks << std::endl;
        // Get the superblock header
        NFS2_SUPERBLOCK *superblock = &superblocks[superBlock_Idx];
        ar.clear();
        ar.seekg(superblockOffsets[superBlock_Idx], ios_base::beg);
        ar.read((char *) &superblock->superBlockSize, sizeof(uint32_t));
        ar.read((char *) &superblock->nBlocks, sizeof(uint32_t));
        ar.read((char *) &superblock->padding, sizeof(uint32_t));

        if (superblock->nBlocks != 0) {
            // Get the offsets of the child blocks within superblock
            uint32_t *blockOffsets = (uint32_t *) calloc(static_cast<size_t>(superblock->nBlocks), sizeof(uint32_t));
            ar.read((char *) blockOffsets, superblock->nBlocks * sizeof(uint32_t));
            superblock->trackBlocks = (NFS2_TRKBLOCK *) calloc(static_cast<size_t>(superblock->nBlocks), sizeof(NFS2_TRKBLOCK));

            for (int block_Idx = 0; block_Idx < superblock->nBlocks; ++block_Idx) {
                std::cout << "  Block " << block_Idx+1 << " of " << superblock->nBlocks << std::endl;
                NFS2_TRKBLOCK *trackblock = &superblock->trackBlocks[block_Idx];
                // Read Header
                trackblock->header = (NFS2_TRKBLOCK_HEADER *) calloc(1, sizeof(NFS2_TRKBLOCK_HEADER));
                ar.seekg(superblockOffsets[superBlock_Idx] + blockOffsets[block_Idx], ios_base::beg);
                ar.read((char *) trackblock->header, sizeof(NFS2_TRKBLOCK_HEADER));

                // Sanity Checks
                if((trackblock->header->blockSize != trackblock->header->blockSizeDup)||(trackblock->header->blockSerial > nBlocks)){
                    std::cout<< "   --- Bad Block" << std::endl;
                    free(blockReferenceCoords);
                    return false;
                }

                // Read 3D Data
                trackblock->vertexTable = (NFS2_VERT *) calloc(static_cast<size_t>(trackblock->header->nStickToNextVerts + trackblock->header->nHighResVert), sizeof(NFS2_VERT));
                for(int vert_Idx = 0; vert_Idx < trackblock->header->nStickToNextVerts + trackblock->header->nHighResVert; ++vert_Idx){
                    ar.read((char *) &trackblock->vertexTable[vert_Idx], sizeof(NFS2_VERT));
                }

                trackblock->polygonTable = (NFS2_POLYGONDATA *) calloc(static_cast<size_t>(trackblock->header->nLowResPoly + trackblock->header->nMedResPoly + trackblock->header->nHighResPoly), sizeof(NFS2_POLYGONDATA));
                for(int poly_Idx = 0; poly_Idx < (trackblock->header->nLowResPoly + trackblock->header->nMedResPoly + trackblock->header->nHighResPoly); ++poly_Idx){
                    ar.read((char *) &trackblock->polygonTable[poly_Idx], sizeof(NFS2_POLYGONDATA));
                }

                // Read Extrablock data
                ar.seekg(superblockOffsets[superBlock_Idx] + blockOffsets[block_Idx] + 64u + trackblock->header->extraBlockTblOffset, ios_base::beg);
                // Get extrablock offsets (relative to beginning of TrackBlock)
                uint32_t *extrablockOffsets = (uint32_t *) calloc(trackblock->header->nExtraBlocks, sizeof(uint32_t));
                ar.read((char *) extrablockOffsets, trackblock->header->nExtraBlocks * sizeof(uint32_t));

                for(int xblock_Idx = 0; xblock_Idx < trackblock->header->nExtraBlocks; ++xblock_Idx){
                    ar.seekg(superblockOffsets[superBlock_Idx] + blockOffsets[block_Idx] + extrablockOffsets[xblock_Idx], ios_base::beg);
                    NFS2_EXTRABLOCK_HEADER *xblockHeader = (NFS2_EXTRABLOCK_HEADER *) calloc(1, sizeof(NFS2_EXTRABLOCK_HEADER));
                    ar.read((char*) xblockHeader, sizeof(NFS2_EXTRABLOCK_HEADER));
                    switch(xblockHeader->XBID){
                        case 5:
                            trackblock->polyTypes = (NFS2_POLY_TYPE *) calloc(xblockHeader->nRecords, sizeof(NFS2_POLY_TYPE));
                            ar.read((char *) trackblock->polyTypes, xblockHeader->nRecords * sizeof(NFS2_POLY_TYPE));
                            break;
                        case 4:
                            trackblock->nNeighbours = xblockHeader->nRecords;
                            trackblock->blockNeighbours = (uint16_t *) calloc(xblockHeader->nRecords, sizeof(uint16_t));
                            ar.read((char*) trackblock->blockNeighbours, xblockHeader->nRecords * sizeof(uint16_t));
                            break;
                        case 8:
                            trackblock->structures = (NFS2_3D_BLOCK *) calloc(xblockHeader->nRecords, sizeof(NFS2_3D_BLOCK));
                            trackblock->nStructures = xblockHeader->nRecords;
                            for(int structure_Idx = 0; structure_Idx < trackblock->nStructures; ++structure_Idx){
                                ar.read((char*) &trackblock->structures[structure_Idx].recSize, sizeof(uint32_t));
                                ar.read((char*) &trackblock->structures[structure_Idx].nVerts, sizeof(uint16_t));
                                ar.read((char*) &trackblock->structures[structure_Idx].nPoly, sizeof(uint16_t));
                                trackblock->structures[structure_Idx].vertexTable = (NFS2_VERT *) calloc(trackblock->structures[structure_Idx].nVerts, sizeof(NFS2_VERT));
                                for(int vert_Idx = 0; vert_Idx < trackblock->structures[structure_Idx].nVerts; ++vert_Idx){
                                    ar.read((char *) &trackblock->structures[structure_Idx].vertexTable[vert_Idx], sizeof(NFS2_VERT));
                                }
                                trackblock->structures[structure_Idx].polygonTable = (NFS2_POLYGONDATA *) calloc(trackblock->structures[structure_Idx].nPoly, sizeof(NFS2_POLYGONDATA));
                                for(int poly_Idx = 0; poly_Idx < trackblock->structures[structure_Idx].nPoly; ++poly_Idx){
                                    ar.read((char *) &trackblock->structures[structure_Idx].polygonTable[poly_Idx], sizeof(NFS2_POLYGONDATA));
                                }
                            }
                            break;
                        case 7:
                        case 18:
                            trackblock->structureRefData = (NFS2_3D_REF_BLOCK *) calloc(xblockHeader->nRecords, sizeof(NFS2_3D_REF_BLOCK));
                            trackblock->nStructureReferences = xblockHeader->nRecords;
                            for(int structureRef_Idx = 0; structureRef_Idx < trackblock->nStructureReferences; ++structureRef_Idx){
                                ar.read((char*) &trackblock->structureRefData[structureRef_Idx].recSize, sizeof(uint16_t));
                                ar.read((char*) &trackblock->structureRefData[structureRef_Idx].recType, sizeof(uint8_t));
                                ar.read((char*) &trackblock->structureRefData[structureRef_Idx].structureRef, sizeof(uint8_t));
                                // Fixed type
                                if(trackblock->structureRefData[structureRef_Idx].recType == 1){
                                    ar.read((char*) &trackblock->structureRefData[structureRef_Idx].refCoordinates, sizeof(NFS2_VERT_HIGH));
                                }
                                else if(trackblock->structureRefData[structureRef_Idx].recType == 3){ // Animated type
                                    ar.read((char*) &trackblock->structureRefData[structureRef_Idx].animLength, sizeof(uint16_t));
                                    ar.read((char*) &trackblock->structureRefData[structureRef_Idx].unknown, sizeof(uint16_t));
                                    trackblock->structureRefData[structureRef_Idx].animationData = (NFS2_ANIM_POS *) calloc(trackblock->structureRefData[structureRef_Idx].animLength, sizeof(NFS2_ANIM_POS));
                                    for(int animation_Idx = 0; animation_Idx < trackblock->structureRefData[structureRef_Idx].animLength; ++animation_Idx){
                                        ar.read((char*) &trackblock->structureRefData[structureRef_Idx].animationData[animation_Idx], sizeof(NFS2_ANIM_POS));
                                    }
                                } else {
                                    std::cout << "Unknown Structure Reference type: " << trackblock->structureRefData[structureRef_Idx].recType << std::endl;
                                }
                            }
                            break;
                        case 6:
                            trackblock->medianData = (NFS2_MEDIAN_BLOCK *) calloc(xblockHeader->nRecords, sizeof(NFS2_MEDIAN_BLOCK));
                            ar.read((char *) trackblock->medianData, xblockHeader->nRecords * sizeof(NFS2_MEDIAN_BLOCK));
                            break;
                        case 13:
                            trackblock->nVroad = xblockHeader->nRecords;
                            trackblock->vroadData = (NFS2_VROAD *) calloc(xblockHeader->nRecords, sizeof(NFS2_VROAD));
                            ar.read((char *) trackblock->vroadData, trackblock->nVroad * sizeof(NFS2_VROAD));
                            break;
                        case 9:
                            trackblock->nLanes = xblockHeader->nRecords;
                            trackblock->laneData = (NFS2_LANE_BLOCK *) calloc(xblockHeader->nRecords, sizeof(NFS2_LANE_BLOCK));
                            ar.read((char *) trackblock->laneData, trackblock->nLanes * sizeof(NFS2_LANE_BLOCK));
                            break;
                        default:
                            std::cout << "Unknown XBID: " << xblockHeader->XBID << std::endl;
                            break;
                    }
                }
            }
        }
    }
    dbgPrintVerts(superblocks, nSuperBlocks, nBlocks, blockReferenceCoords, "C:/Users/Amrik/Desktop/trk.obj", false);
    ar.close();
}

void dbgPrintVerts(NFS2_SUPERBLOCK *superblocks, long nSuperBlocks, long nBlocks, NFS2_VERT_HIGH *blockReferenceCoords, std::string path, bool printFaces) {
    std::ofstream obj_dump;
    obj_dump.open(path);

    for(int superBlock_Idx = 0; superBlock_Idx < nSuperBlocks; ++superBlock_Idx){
        NFS2_SUPERBLOCK superblock = superblocks[superBlock_Idx];
        for (int block_Idx = 0; block_Idx < superblock.nBlocks; ++block_Idx) {
            NFS2_TRKBLOCK trkBlock = superblock.trackBlocks[block_Idx];
            NFS2_VERT_HIGH blockReferenceCoord;
            // Print clipping rectangle
            obj_dump << "o Block" << trkBlock.header->blockSerial << "ClippingRect" << std::endl;
            for(int i = 0; i < 4; i++){
                obj_dump << "v " << trkBlock.header->clippingRect[i].x << " " << trkBlock.header->clippingRect[i].z << " " << trkBlock.header->clippingRect[i].y << std::endl;
            }
            obj_dump << "o Block" << trkBlock.header->blockSerial << std::endl;
            for (int i = 0; i < trkBlock.header->nStickToNextVerts + trkBlock.header->nHighResVert; i++) {
                if (i < trkBlock.header->nStickToNextVerts) {
                    // If in last block go get ref coord of first block, else get ref of next block
                    blockReferenceCoord =  (trkBlock.header->blockSerial == nBlocks-1) ? blockReferenceCoords[0] :  blockReferenceCoords[trkBlock.header->blockSerial+1];
                } else {
                    blockReferenceCoord = blockReferenceCoords[trkBlock.header->blockSerial];
                }
                    double x = (double)(blockReferenceCoord.x + (uint32_t) (256 * trkBlock.vertexTable[i].x))/10000000;
                    double y = (double)(blockReferenceCoord.y + (uint32_t) (256 * trkBlock.vertexTable[i].y))/10000000;
                    double z = (double)(blockReferenceCoord.z + (uint32_t) (256 * trkBlock.vertexTable[i].z))/10000000;
                    obj_dump << "v " << x << " " << z << " " << y << std::endl;
            }
            // TODO: Why are track poly's so screwed?
            if(printFaces){
                for (unsigned int poly_Idx = (trkBlock.header->nLowResPoly + trkBlock.header->nMedResPoly); poly_Idx < (trkBlock.header->nLowResPoly + trkBlock.header->nMedResPoly + trkBlock.header->nHighResPoly); ++poly_Idx)
                {
                    obj_dump << "f " << (unsigned int) trkBlock.polygonTable[poly_Idx].vertex[0]+1 << " " << (unsigned int)trkBlock.polygonTable[poly_Idx].vertex[1]+1 << " " << (unsigned int) trkBlock.polygonTable[poly_Idx].vertex[2]+1 << std::endl;
                    obj_dump << "f " << (unsigned int) trkBlock.polygonTable[poly_Idx].vertex[0]+1 << " " << (unsigned int)trkBlock.polygonTable[poly_Idx].vertex[2]+1 << " " << (unsigned int) trkBlock.polygonTable[poly_Idx].vertex[3]+1 << std::endl;
                }
            }
            for(int structure_Idx = 0; structure_Idx < trkBlock.nStructures; ++structure_Idx){
                NFS2_VERT_HIGH structureReferenceCoordinates = blockReferenceCoords[trkBlock.header->blockSerial];
                // Find the structure reference that matches this structure, else use block default
                for(int structRef_Idx = 0; structRef_Idx < trkBlock.nStructureReferences; ++structRef_Idx){
                    // Only check fixed type structure references
                    if(trkBlock.structureRefData[structRef_Idx].recType == 1){
                        if(trkBlock.structureRefData[structRef_Idx].structureRef == structure_Idx){
                            structureReferenceCoordinates = trkBlock.structureRefData[structure_Idx].refCoordinates;
                        }
                    }
                }
                obj_dump << "o Struct" << structure_Idx << std::endl;
                for(int vert_Idx = 0; vert_Idx < trkBlock.structures[structure_Idx].nVerts; ++vert_Idx){
                    double x = (double)(structureReferenceCoordinates.x + (uint32_t) (256 * trkBlock.structures[structure_Idx].vertexTable[vert_Idx].x))/10000000;
                    double y = (double)(structureReferenceCoordinates.y + (uint32_t) (256 *trkBlock.structures[structure_Idx].vertexTable[vert_Idx].y))/10000000;
                    double z = (double)(structureReferenceCoordinates.z + (uint32_t) (256 *trkBlock.structures[structure_Idx].vertexTable[vert_Idx].z))/10000000;
                    obj_dump << "v " << x << " " << z << " " << y << std::endl;
                }
                if(printFaces){
                    for(int poly_Idx = 0; poly_Idx < trkBlock.structures[structure_Idx].nPoly; ++poly_Idx){
                        obj_dump << "f " << (unsigned int) trkBlock.structures[structure_Idx].polygonTable[poly_Idx].vertex[0]+1 << " " << (unsigned int)trkBlock.structures[structure_Idx].polygonTable[poly_Idx].vertex[1]+1 << " " << (unsigned int) trkBlock.structures[structure_Idx].polygonTable[poly_Idx].vertex[2]+1 << std::endl;
                        obj_dump << "f " << (unsigned int) trkBlock.structures[structure_Idx].polygonTable[poly_Idx].vertex[0]+1 << " " << (unsigned int)trkBlock.structures[structure_Idx].polygonTable[poly_Idx].vertex[2]+1 << " " << (unsigned int) trkBlock.structures[structure_Idx].polygonTable[poly_Idx].vertex[3]+1 << std::endl;
                    }
                }
            }
        }
    }
    obj_dump.close();
}
