#include "SpeedsFile.h"

using namespace LibOpenNFS::NFS3;

bool SpeedsFile::Load(const std::string& speedBinPath, SpeedsFile& speedFile)
{
    LOG(INFO) << "Loading FRD File located at " << speedBinPath;
    std::ifstream speedBin{speedBinPath, std::ios::binary};

    bool loadStatus = speedFile.SerializeIn(speedBin);
    speedBin.close();

    return loadStatus;
}

void SpeedsFile::Save(const std::string& speedBinPath, SpeedsFile& speedFile)
{
    LOG(INFO) << "Saving SPEED BIN File to " << speedBinPath;
    std::ofstream speedBin{speedBinPath, std::ios::binary};
    speedFile.SerializeOut(speedBin);
}

bool SpeedsFile::SerializeIn(std::istream& ifstream)
{
    // Tactical grab of the file size
    ifstream.ignore(std::numeric_limits<std::streamsize>::max());
    m_uFileSize = ifstream.gcount();
    ifstream.clear();
    ifstream.seekg(0, std::ios_base::beg);

    speeds.resize(m_uFileSize);
    SAFE_READ(ifstream, speeds.data(), m_uFileSize);

    return true;
}

void SpeedsFile::SerializeOut(std::ostream& ofstream)
{
    ofstream.write((char*) speeds.data(), m_uFileSize);
}

void SpeedsFile::SaveCSV(const std::string& speedsCsvPath, SpeedsFile& speedFile)
{
    LOG(INFO) << "Saving SPEED BIN File to CSV: " << speedsCsvPath;
    std::ofstream speedCsv{speedsCsvPath, std::ios::binary};

    for (auto& speed : speedFile.speeds)
    {
        speedCsv << static_cast<uint16_t>(speed) << "," << std::endl;
    }

    speedCsv.close();
}
