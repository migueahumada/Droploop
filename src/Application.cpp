#include "Application.h"
#include <stdio.h>

Application::Application() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(0);
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
    window = glfwCreateWindow(WIN_HEIGHT, WIN_WIDTH, TITLE, nullptr, nullptr);
    if (window == nullptr)
        exit(0);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

Application::~Application() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::Run() {

   

    

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

        //-----------Window for Audio Player-------

        ImGui::Begin("Player");
        ImGui::Text("This is an audio player");

        audioLoader.load(currentSound.get());
        
        ImGui::Text(audioLoader.getArtist(currentSound.get()).c_str());
        ImGui::Text(std::to_string(audioLoader.getLength(currentSound.get())).c_str());
        
        std::cout << currentSound.get()->m_Info.frames<< std::endl;
        
        if (ImGui::Button("Play")){
            audioLoader.load(testSound.get());
            audioIO.play(testSound.get());
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            audioLoader.load(testSound.get());
            audioIO.play(testSound.get());
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause")) {
            audioLoader.load(testSound.get());
            audioIO.play(testSound.get());
        }

        bool columnsSelected[NUM_COLUMNS] = {true,false,false,false};
        
        if (ImGui::BeginTable("Audio", NUM_COLUMNS, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_SizingFixedSame))
        {

            ImGui::TableSetupColumn("Category");
            ImGui::TableSetupColumn("Artist");
            ImGui::TableSetupColumn("Length (s)");
            ImGui::TableSetupColumn("Title");
            ImGui::TableHeadersRow();

            ImGui::TableNextColumn();
            ImGui::Selectable(audioLoader.getCategory(currentSound.get()).c_str());
                
            ImGui::TableNextColumn();
            ImGui::Selectable(audioLoader.getArtist(currentSound.get()).c_str());

            ImGui::TableNextColumn();
            ImGui::Selectable(std::to_string(audioLoader.getLength(currentSound.get())).c_str());

            ImGui::TableNextColumn();
            ImGui::Selectable(audioLoader.getTitle(currentSound.get()).c_str());

            ImGui::EndTable();
        }
        ImGui::End();

        //-----------End Window for Audio Player-------

        //Shows the Demo Stuff
        ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

}

void Application::glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}