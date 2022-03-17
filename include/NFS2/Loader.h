#pragma once

#include "NFS2/GEO/GeoFile.h"
#include "NFS2/TRK/TrkFile.h"
#include "NFS2/COL/ColFile.h"

const float NFS2_SCALE_FACTOR = 1000000.0f;

template <typename Platform>
class NFS2Loader
{
public:
    static std::shared_ptr<Car> LoadCar(const std::string &carBasePath, NFSVer nfsVersion);
    static std::shared_ptr<Track> LoadTrack(const std::string &trackBasePath, NFSVer nfsVersion);

private:
    static CarData _ParseGEOModels(const LibOpenNFS::NFS2::GeoFile<Platform> &geoFile);
    static std::vector<OpenNFS::TrackBlock> _ParseTRKModels(const LibOpenNFS::NFS2::TrkFile<Platform> &trkFile, LibOpenNFS::NFS2::ColFile<Platform> &colFile,
                                                            const std::shared_ptr<Track> &track);
    static std::vector<VirtualRoad> _ParseVirtualRoad(LibOpenNFS::NFS2::ColFile<Platform> &colFile);
    static std::vector<Entity> _ParseCOLModels(LibOpenNFS::NFS2::ColFile<Platform> &colFile, const std::shared_ptr<Track> &track);
};
