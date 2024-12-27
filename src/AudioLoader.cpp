#include "AudioLoader.h"

AudioLoader::AudioLoader() {
	// Constructor
}

void AudioLoader::load(SoundFile* soundfile) {
	soundfile->m_File = sf_open(soundfile->m_Path, SFM_RDWR, &soundfile->m_Info);
	if (sf_error(soundfile->m_File) != SF_ERR_NO_ERROR)
	{
		printf("Error while opening soundfile -> %s\n", sf_strerror(soundfile->m_File));
		
	}
	else {
		printf("Channels -> %d\n", static_cast<int>(soundfile->m_Info.frames));
	}
}