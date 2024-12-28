#pragma once
#include <memory>
#include <iostream>
#include "sndfile.h"

class SoundFile {
public:
	explicit SoundFile(const char* pathName);
	~SoundFile();

	//TODO: Repair error
	SNDFILE* m_File;
	SF_INFO m_Info;
	const char* m_Path;
};