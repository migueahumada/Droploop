#pragma once

#include "SoundFile.h"
#include <map>
#include <memory>
#include <string>

class AudioLoader {
public:
	AudioLoader();
	~AudioLoader() = default;

	void load(SoundFile* soundfile);
private:
};