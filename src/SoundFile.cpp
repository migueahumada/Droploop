#include "SoundFile.h"

SoundFile::SoundFile(const char* pathName) : m_Path(pathName){

    memset(&m_Info, 0, sizeof(m_Info));

    m_File = sf_open(pathName,SFM_RDWR,&m_Info);
    if (!m_File)
    {
        std::cerr << "Error: " << sf_strerror(m_File) << std::endl;
    }
    std::cout << "Audio creado!" << std::endl;
}

SoundFile::~SoundFile() {

    sf_close(m_File);
    memset(&m_Info,0,sizeof(m_Info));

    std::cout << "Audio destruido" << std::endl;
    
    
}



