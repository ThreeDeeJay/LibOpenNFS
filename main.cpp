#include <iostream>
#include <fstream>

#include "NFS2/COL/ColFile.h"
#include "NFS2/PSH/PshFile.h"
#include "NFS2/TRK/TrkFile.h"

#include "NFS5/CRP/CRPFile.h"
#include "NFS5/VIV/VIVFile.h"

int main()
try
{
    std::ifstream fin{"zzzymus.viv", std::ios::binary};

    LibOpenNFS::NFS5::VIVFile c;
    
    fin >> c;

    std::ofstream fout{"a.bnk", std::ios::binary};

    fout.write(c.files[0].data.data(), c.files[0].data.size());

}
catch(std::exception& e)
{
    std::cerr << e.what();
}