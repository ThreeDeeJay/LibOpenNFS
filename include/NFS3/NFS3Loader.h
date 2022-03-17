#pragma once

#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cstdlib>

#include "FCE/FceFile.h"
#include "FEDATA/FedataFile.h"
#include "FRD/FrdFile.h"
#include "COL/ColFile.h"
#include "SPEEDS/SpeedsFile.h"
#include "../Shared/CanFile.h"
#include "../Shared/HrzFile.h"
#include "../Common/TrackUtils.h"
#include "../../Config.h"
#include "../../Util/Utils.h"
#include "../../Physics/Car.h"
#include "../../Scene/Track.h"
#include "../../Scene/VirtualRoad.h"
#include "../../Scene/TrackBlock.h"
#include "../../Renderer/Texture.h"

const float NFS3_SCALE_FACTOR = 10.f;

namespace LibOpenNFS::NFS3::Loader
{
    std::shared_ptr<Car> LoadCar(const std::string &carBasePath);
    std::shared_ptr<Track> LoadTrack(const std::string &trackBasePath);
};
