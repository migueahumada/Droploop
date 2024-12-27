#pragma once
#include <portaudio.h>
#include <iostream>

#include "SoundFile.h"

class AudioIO {
public:
	AudioIO();
	~AudioIO();
	static int callbackFunction(const void* input, void* output, 
						unsigned long frameCount, 
						const PaStreamCallbackTimeInfo* timeInfo,
						PaStreamCallbackFlags statusFlags,
						void* userData);
	void initDeviceInfo();
	void play(SoundFile* soundfile);

	double m_SampleRate;
private:
	PaError paError;
	const PaDeviceInfo* m_DeviceInfo;
	PaStreamParameters m_OutParams;
	PaStreamParameters m_InParams;
	PaStream* m_Stream;
	
	int m_FramesPerBuffer;
};