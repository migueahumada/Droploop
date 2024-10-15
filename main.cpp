// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

//Learn about libsndfile:
// - Documentation        http://www.mega-nerd.com/libsndfile/api.html
// - Documentation        https://svn.ict.usc.edu/svn_vh_public/trunk/lib/vhcl/libsndfile/doc/api.html


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

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


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
    SF_INFO* psfinfo_in;
    SNDFILE* psf_in;
    SF_FORMAT_INFO	format_info;
    int             k, count;
    

    psfinfo_in = (SF_INFO*)malloc(sizeof(SF_INFO));
    psf_in = sf_open("D:/Coding/C++/Droploop/assets/Audio_01.wav", SFM_RDWR, psfinfo_in);
    
    if (psf_in)
    {
        std::cout << "Soundfile open!\n";
    }
    sf_set_string(psf_in, SF_STR_TITLE,"Test Audio");
    printf("%s",sf_get_string(psf_in, SF_STR_TITLE));

    char buffer[64];
    char buffer2[256];
    memset(buffer2, 0, sizeof(buffer2));
    const char* placeholder;

    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
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

        ImGui::Begin("Table");
        ImGui::BeginTable("TABLE",4,ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg,ImVec2(120.4f,50.0f));
        for (int row = 0; row < 3; row++)
        {
            ImGui::TableNextRow(0, 520.0f * 1.5f);
            for (int column = 0; column < 3; column++)
            {
                ImGui::TableNextColumn();
                ImGui::Text("Cell %d,%d", column, row);
            }
        }
        ImGui::EndTable();
        
        ImGui::End();

        //Another 
        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("Audio Information"); 
        if (psfinfo_in)
        {
            
            // Assuming psfinfo_in->samplerate is an integer (e.g., int or unsigned int)
            


            snprintf(buffer, sizeof(buffer), "Title: %s", sf_get_string(psf_in,SF_STR_TITLE));
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

        
        

        /*
        struct SF_INFO
{	sf_count_t	frames ;		
        int			samplerate;
        int			channels;
        int			format;
        int			sections;
        int			seekable;
    };*/
        
        
        ImGui::End();
        

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

    sf_close(psf_in);
    free(psfinfo_in);
    

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
