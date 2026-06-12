#include "taskbar.h"
#include "task_manager.h"
#include "imgui-docking/backends/imgui_impl_glfw.h"
#include "imgui-docking/backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <cstdio>

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 800, "CSOPESY Desktop Emulator", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ApplyStyle();

    AppState state;

    std::vector<Process> processes = {
        {1240,  "C+G", "C:\\Program Files\\WindowsApps\\...\\WindowsTerminal.exe", "128 MB"},
        {2344,  "C+G", "D:\\Steam\\bin\\cef\\cef.win64\\steamwebhelper.exe",        "256 MB"},
        {9944,  "C+G", "C:\\Windows\\explorer.exe",                                 "64 MB"},
        {17828, "C+G", "C:\\Program Files\\WindowsApps\\...\\iCloudHome.exe",       "80 MB"},
        {20816, "C+G", "C:\\Users\\User\\AppData\\Roaming\\Spotify\\Spotify.exe",   "312 MB"},
    };

    const float bg_r = ((Pal::BG >> 16) & 0xFF) / 255.f;
    const float bg_g = ((Pal::BG >> 8) & 0xFF) / 255.f;
    const float bg_b = (Pal::BG & 0xFF) / 255.f;

    while (!glfwWindowShouldClose(window) && state.running) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        state.time += io.DeltaTime;

        float sw = io.DisplaySize.x;
        float sh = io.DisplaySize.y;

        DrawTaskbar(state, sw, sh);
        DrawTaskManager(state.showTaskManager, sw, sh, processes);
        DrawSystemInfo(state, sw, sh);

        ImGui::Render();
        int fb_w, fb_h;
        glfwGetFramebufferSize(window, &fb_w, &fb_h);
        glViewport(0, 0, fb_w, fb_h);
        glClearColor(bg_r, bg_g, bg_b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}