#include "FileLoader.h"

FileLoader::FileLoader(){
    
}

void FileLoader::LoadFilesFromDirectory(){
    if (std::filesystem::exists(m_DefaultDirectory) && std::filesystem::is_directory(m_DefaultDirectory))
    {
        for (const auto& entry : std::filesystem::directory_iterator(m_DefaultDirectory)) {
            std::cout << "File: " << entry.path() << std::endl;
            m_SoundFiles.push_back(std::make_shared<SoundFile>(entry.path().string().c_str()));
        }
    }

    for (const auto& entry : m_SoundFiles)
    {
        std::cout << "File as string: " << entry->m_Path << std::endl;
    }
}