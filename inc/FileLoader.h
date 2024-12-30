#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include "SoundFile.h"

class FileLoader {
public:
	FileLoader();
	~FileLoader() = default;

	

	void LoadFilesFromDirectory();

	std::vector<std::shared_ptr<SoundFile>> m_SoundFiles;
	std::filesystem::path m_DefaultDirectory = "D:/Coding/C++/Droploop/assets/";

};