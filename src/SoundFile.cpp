#include "SoundFile.h"

SoundFile::SoundFile(const char* pathName) : m_Path(pathName){
    m_File = nullptr;
    memset(&m_Info, 0, sizeof(m_Info));
}
SoundFile::~SoundFile() {
	sf_close(m_File);
    m_File = nullptr;
    memset(&m_Info, 0, sizeof(m_Info));
}



