#pragma once

#include "SoundFile.h"
#include <map>
#include <memory>
#include <string>

class AudioLoader {
public:
	AudioLoader();
	~AudioLoader() = default;

	//CART INFO CHUNK functions
	std::string getTitle(SoundFile* soundfile) const;
	std::string getArtist(SoundFile* soundfile) const;
	std::string getCategory(SoundFile* soundfile) const;
	double getLength(SoundFile* soundfile) const;
private:
};