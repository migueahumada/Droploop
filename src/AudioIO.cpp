#include "AudioIO.h"
#define _USE_MATH_DEFINES
#include <math.h>

AudioIO::AudioIO() {
    memset(&m_OutParams, 0, sizeof(PaStreamParameters));
    memset(&m_InParams, 0, sizeof(PaStreamParameters));
    m_DeviceInfo = nullptr;
    m_Stream = nullptr;
    m_SampleRate = 48000;
    m_FramesPerBuffer = 512;

    paError = Pa_Initialize();
    if (paError != paNoError)
        std::cout << "Error: " << Pa_GetErrorText(paError) << std::endl;
}

AudioIO::~AudioIO() {
    paError = Pa_Terminate();
    if (paError != paNoError)
        std::cout << "Error: " << Pa_GetErrorText(paError) << std::endl;
}

int AudioIO::callbackFunction(const void* inBuffer, void* outBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData) {

    SoundFile* data = reinterpret_cast<SoundFile*>(userData);
    float* outData = reinterpret_cast<float*>(outBuffer);
    static_cast<void>(inBuffer);
    
    int readFrames = sf_readf_float(data->m_File, outData, framesPerBuffer);

    if (readFrames == 0)
    {
        return paComplete;
    }

    return paContinue;
    
    
}

void AudioIO::initDeviceInfo() {
    if (Pa_GetDefaultInputDevice() == paNoDevice || Pa_GetDefaultOutputDevice() == paNoDevice) {
        std::cout << "Error: No default input or output device found!" << std::endl;
        return;
    }

    std::cout << Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->name << std::endl;
    std::cout << Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->maxInputChannels << std::endl;
    std::cout << Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->name << std::endl;
    std::cout << Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->maxOutputChannels << std::endl;

    m_InParams.device = Pa_GetDefaultInputDevice();
    m_InParams.channelCount = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->maxInputChannels;
    m_InParams.sampleFormat = paFloat32;
    m_InParams.hostApiSpecificStreamInfo = nullptr;
    m_InParams.suggestedLatency = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->defaultHighInputLatency;

    m_OutParams.device = Pa_GetDefaultOutputDevice();
    m_OutParams.channelCount = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->maxOutputChannels;
    m_OutParams.hostApiSpecificStreamInfo = nullptr;
    m_OutParams.sampleFormat = paFloat32;
    m_OutParams.suggestedLatency = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->defaultHighOutputLatency;
}

void AudioIO::play(SoundFile* soundfile) {
    if (soundfile == nullptr) {
        std::cout << "Error: soundfile is null!" << std::endl;
        return;
    }

    m_SampleRate = static_cast<double>(soundfile->m_Info.samplerate);

    paError = Pa_OpenDefaultStream(&m_Stream, 0, 2, paFloat32, m_SampleRate, m_FramesPerBuffer, callbackFunction, soundfile);
    //paError = Pa_OpenStream(&m_Stream, 0, &, paFloat32, m_SampleRate, m_FramesPerBuffer, callbackFunction, soundfile);
    if (paError != paNoError)
        std::cout << "Error opening stream: " << Pa_GetErrorText(paError) << std::endl;

    paError = Pa_StartStream(m_Stream);
    if (paError != paNoError)
        std::cout << "Error starting stream: " << Pa_GetErrorText(paError) << std::endl;

    //std::cout << Pa_GetErrorText(paError) << ": "<< paError;
    
}
