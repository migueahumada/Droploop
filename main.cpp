
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <sndfile.h>
#include <portaudio.h>
#include <cstring>
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

typedef struct SoundfileData 
{
    SNDFILE* soundfile;
    SF_INFO soundfileInfo;
}SoundfileData;

static inline float max(float a, float b) {
    return a > b ? a : b;
}

static int emptyCallback(const void* inputBuffer,
                        void* outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData)
{
        SoundfileData* sfData = (SoundfileData*)userData;
    float* outBuffer = (float*)outputBuffer;

    sf_count_t samplesRead = sf_read_float(sfData->soundfile, outBuffer, framesPerBuffer * sfData->soundfileInfo.channels);;
    if (samplesRead < framesPerBuffer * sfData->soundfileInfo.channels)
    {
        memset(outBuffer+samplesRead, 0, (framesPerBuffer * sfData->soundfileInfo.channels - samplesRead) * sizeof(float));
        return paComplete;
    }

    return paContinue;
}

static int paTestCallback(const void* inputBuffer, 
                      void* outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags, 
                      void* userData) 
{
    float* in = (float*)inputBuffer;
    (void)outputBuffer;

    int dispSize = 100;
    printf("\r");

    float vol_l = 0;
    float vol_r = 0;

    for (unsigned long i = 0; i < framesPerBuffer * 2; i += 2) {
        vol_l = max(vol_l, std::abs(in[i]));
        vol_r = max(vol_r, std::abs(in[i + 1]));
    }

    for (int i = 0; i < dispSize; i++) {
        float barProportion = i / (float)dispSize;
        if (barProportion <= vol_l && barProportion <= vol_r) {
            printf("%c",219);
        }
        else if (barProportion <= vol_l) {
            printf("%c",220);
        }
        else if (barProportion <= vol_r) {
            printf("%c",223);
        }
        else {
            printf(" ");
        }
    }

    fflush(stdout);

    return 0;

}


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void SetTitleMetada(SNDFILE* soundfile, const char* title) 
{
    sf_set_string(soundfile,SF_STR_TITLE,title);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //------------------------------AudioStuff-------------------------------------
    /* Libsndfile */
    SF_INFO* psfinfo_in;
    SNDFILE* psf_in;
    SF_FORMAT_INFO format_info;
    int k, count;

    /* Portaudio */
    PaError paError;
    const PaDeviceInfo* deviceInfo;
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    PaStream* stream;
    double sampleRate = 44100.0;
    int framesPerBuffer = 512;
    int inputDevice = 1;
    int outputDevice = 6;
    bool isPlaying = false;

    paError = Pa_Initialize();
    if (paError != paNoError)
    {
        printf("Portaudio couldn't initialize, please restart the application.\n");
    }
    
    if (1)
    {
        for (int i = 0; i < Pa_GetDeviceCount(); i++)
        {
            deviceInfo = Pa_GetDeviceInfo(i);
            printf("-Device %d\n", i);
            printf("\t-Name: %s\n", deviceInfo->name);
            printf("\t-Input channels: %d\n", deviceInfo->maxInputChannels);
            printf("\t-Output channels: %d\n", deviceInfo->maxOutputChannels);
            printf("\t-Sample rate: %.2f\n", deviceInfo->defaultSampleRate);
        }
    }
    
    // Initialize portaudio device info, PA always needs that
    deviceInfo = Pa_GetDeviceInfo(inputDevice);
    if (deviceInfo == NULL)
    {
        printf("Error device info is nullptr");
    }
    memset(&inputParameters, 0, sizeof(inputParameters)); //Initialize all structure parameters to 0
    inputParameters.device = inputDevice;
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency;

    deviceInfo = Pa_GetDeviceInfo(outputDevice);
    if (deviceInfo == NULL)
    {
        printf("Error device info is nullptr");
    }
    memset(&outputParameters, 0, sizeof(outputParameters));
    outputParameters.device = outputDevice;
    outputParameters.channelCount = 6;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;

    /*
    paError = Pa_OpenStream(
        &stream,
        &inputParameters,
        &outputParameters,
        sampleRate,
        framesPerBuffer,
        paNoFlag,
        paTestCallback,
        NULL);
    if (paError != paNoError)
    {
        printf("Error while OPENING a stream. -> %s\n", Pa_GetErrorText(paError));
        return(EXIT_FAILURE);
    }

    paError = Pa_StartStream(stream);
    if (paError != paNoError)
    {
        printf("Error while STARTING a stream-> %s\n", Pa_GetErrorText(paError));
        return(EXIT_FAILURE);
    }

    psfinfo_in = (SF_INFO*)malloc(sizeof(SF_INFO));
    psf_in = sf_open("D:/Coding/C++/Droploop/assets/Audio_01.wav", SFM_RDWR, psfinfo_in);
    
    if (psf_in)
    {
        std::cout << "Soundfile open!\n";
    }
    sf_set_string(psf_in, SF_STR_TITLE,"Test Audio");
    printf("%s",sf_get_string(psf_in, SF_STR_TITLE));
    */

    //------------------------------End Audio Stuff-------------------------------------

    char buffer[64];
    char buffer2[256];
    memset(buffer2, 0, sizeof(buffer2));
    const char* placeholder;
    bool show_demo_window=true;

    while (!glfwWindowShouldClose(window))
    {
        
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        /*BUTTON PLAYER*/
        ImGui::Begin("Audio Player");
        ImGui::SeparatorText("Play a sound!");
        ImVec2 PlayerButtonSize(120.0f, 50.0f);

        if (ImGui::Button("Play", PlayerButtonSize))
        {
            SoundfileData soundfileData;
            
            //Loads file to memory
            printf("The play button was clicked!");
            soundfileData.soundfile = sf_open("D:/Coding/C++/Droploop/assets/Audio_01.wav", SFM_RDWR, &soundfileData.soundfileInfo);
            if (sf_error(soundfileData.soundfile) != SF_ERR_NO_ERROR)
            {
                printf("Error while opening soundfile -> %s\n", sf_strerror(soundfileData.soundfile));
                return(EXIT_FAILURE);
            }
            //Opens a stream to play it
            paError = Pa_OpenStream(
                &stream,
                NULL,
                &outputParameters,
                sampleRate,
                framesPerBuffer,
                paClipOff,
                emptyCallback,
                &soundfileData);
            if (paError != paNoError)
            {
                printf("Error while opening a stream-> %s\n", Pa_GetErrorText(paError));
                return(EXIT_FAILURE);
            }

            paError = Pa_StartStream(stream);
            if (paError != paNoError)
            {
                printf("Error while starting a stream-> %s\n", Pa_GetErrorText(paError));
                return(EXIT_FAILURE);
            }
            
            //Calculate the duration of the soundfile in miliseconds(ms)
            Pa_Sleep((soundfileData.soundfileInfo.frames/ soundfileData.soundfileInfo.samplerate)*1000);

            paError = Pa_StopStream(stream);
            if (paError != paNoError)
            {
                printf("Error while stopping a stream-> %s\n", Pa_GetErrorText(paError));
                return(EXIT_FAILURE);
            }

            paError = Pa_CloseStream(stream);
            if (paError != paNoError)
            {
                printf("Error while closing a stream.-> %s\n", Pa_GetErrorText(paError));
                return(EXIT_FAILURE);
            }

            sf_close(soundfileData.soundfile);
            

        }
        ImGui::SameLine();
        if (ImGui::Button("Stop", PlayerButtonSize))
        {
            printf("The play button was clicked!");
        }
        ImGui::SameLine(); //AddSpace
        if (ImGui::Button("Pause", PlayerButtonSize))
        {
            printf("The play button was clicked!");
        }
        
        ImGui::End();

        /*
        * ImGui::SeparatorText("Sound Information");
        if (psfinfo_in)
        {
            // Assuming psfinfo_in->samplerate is an integer (e.g., int or unsigned int)

            snprintf(buffer, sizeof(buffer), "Title: %s", sf_get_string(psf_in, SF_STR_TITLE));
            ImGui::Text("%s", buffer);

            if (ImGui::InputText("Custom Text", buffer2, sizeof(buffer2)))
            {
                if (sf_set_string(psf_in, SF_STR_TITLE, buffer2) != 0)
                {
                    std::cerr << "Failed to set the title metadata.\n";
                }
                else
                {
                    std::cout << "Title metadata set successfully!\n";
                }
            }

            snprintf(buffer, sizeof(buffer), "Sample Rate: %d", psfinfo_in->samplerate);
            ImGui::Text("%s", buffer);

            snprintf(buffer, sizeof(buffer), "Frames: %d", (int)psfinfo_in->frames);
            ImGui::Text("%s", buffer);

            snprintf(buffer, sizeof(buffer), "Channels: %d", psfinfo_in->channels);
            ImGui::Text("%s", buffer);

            snprintf(buffer, sizeof(buffer), "Format: %d", psfinfo_in->format);
            ImGui::Text("%s", buffer);

            snprintf(buffer, sizeof(buffer), "Sections: %d", psfinfo_in->sections);
            ImGui::Text("%s", buffer);

            snprintf(buffer, sizeof(buffer), "Seekable: %d", psfinfo_in->seekable);
            ImGui::Text("%s", buffer);

        }
        ImGui::End();
        */
      

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //-----------------Audio Cleanup------------
    
    
    paError = Pa_Terminate();
    if (paError != paNoError)
    {
        printf("Error while closing a stream.-> %s\n", Pa_GetErrorText(paError));
        return(EXIT_FAILURE);
    }
    

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
