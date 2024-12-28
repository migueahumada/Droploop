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
}

/*
	Returns the Title on the Cart Chunk of AES46
*/
std::string AudioLoader::getTitle(SoundFile* soundfile) const {

	SF_CART_INFO info;

	sf_command(soundfile->m_File, SFC_GET_CART_INFO, &info, sizeof(SF_CART_INFO));

	return info.title;
}

/*
	Returns the Artist on the Cart Chunk of AES46
*/
std::string AudioLoader::getArtist(SoundFile* soundfile) const {
	
	SF_CART_INFO info;

	sf_command(soundfile->m_File,SFC_GET_CART_INFO, &info, sizeof(SF_CART_INFO));
	
	return info.artist;
}

/*
	Returns the Category on the Cart Chunk of AES46
*/
std::string AudioLoader::getCategory(SoundFile* soundfile) const {

	SF_CART_INFO info;

	sf_command(soundfile->m_File, SFC_GET_CART_INFO, &info, sizeof(SF_CART_INFO));

	return info.category;
}

/*
	Returns the File Length in seconds
*/
double AudioLoader::getLength(SoundFile* soundfile) const {

	return static_cast<double>(soundfile->m_Info.frames/static_cast<double>(soundfile->m_Info.samplerate));

}
//156001 frames / 96000

//940616 frames / 96000 framesPerSecond



