#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#include "AudioLoader.h"
#include "AudioIO.h"
#include <string>
#include <vector>
#include "FileLoader.h"
#include <fftw3.h>




class Application {
public:
	Application();
	~Application();
	void Run();

	static void glfw_error_callback(int error, const char* description);

	GLFWwindow* window;
	const int WIN_HEIGHT = 1280;
	const int WIN_WIDTH = 720;
	const char* TITLE = "Droploop";
	ImVec4 clear_color;
	bool show_demo_window = true;

	//Systems
	AudioLoader audioLoader;
	AudioIO audioIO;
	FileLoader fileLoader;

	static const int NUM_COLUMNS = 5;

	//Placeholder Audio
	std::shared_ptr<SoundFile> testSound = std::make_shared<SoundFile>("D:/Coding/C++/Droploop/assets/Audio_01.wav");
	std::vector<float> soundSamples;

};