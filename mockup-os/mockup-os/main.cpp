#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <cstdio>
#include <string>
#include <vector>


struct Process {
    int         pid;
    std::string status;
    std::string path;
    std::string memory;
};


static void RenderDesktop(GLFWwindow* window, bool& shouldClose);
static void RenderTaskManager(bool& show);


int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);


    // 2. Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "CSOPESY Desktop OS Emulator", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    // 3. Init Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    // 4. App state
    bool shouldClose = false;
    bool showTaskManager = false;


    // 5. Main render loop
    while (!glfwWindowShouldClose(window) && !shouldClose)
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderDesktop(window, shouldClose);
        RenderTaskManager(showTaskManager);

        //Task Manager goes here

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }


    // 6. Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


static void RenderDesktop(GLFWwindow* window, bool& shouldClose)
{
    int screenW, screenH;
    glfwGetWindowSize(window, &screenW, &screenH);


    // A. Full-screen invisible canvas window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)screenW, (float)screenH));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGuiWindowFlags desktopFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoInputs;

    ImGui::Begin("##Desktop", nullptr, desktopFlags);

    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 p0 = ImVec2(0, 0);
    ImVec2 p1 = ImVec2((float)screenW, (float)screenH);


    // B. Gradient wallpaper (midnight blue → amber-orange)
    const ImU32 colorTopLeft = IM_COL32(13, 27, 42, 255);
    const ImU32 colorTopRight = IM_COL32(22, 38, 74, 255);
    const ImU32 colorBottomRight = IM_COL32(180, 80, 20, 255);
    const ImU32 colorBottomLeft = IM_COL32(201, 106, 31, 255);

    draw->AddRectFilledMultiColor(p0, p1,
        colorTopLeft, colorTopRight,
        colorBottomRight, colorBottomLeft);


    // C. Star field (static seeded dots, upper 55%)
    {
        const int   STAR_COUNT = 120;
        const float STAR_ZONE_H = (float)screenH * 0.55f;

        unsigned int seed = 0xDEADBEEF;
        auto lcg = [&]() -> float {
            seed = seed * 1664525u + 1013904223u;
            return (float)(seed & 0xFFFF) / 65535.0f;
            };

        for (int i = 0; i < STAR_COUNT; ++i)
        {
            float x = lcg() * (float)screenW;
            float y = lcg() * STAR_ZONE_H;
            float r = lcg() * 1.5f + 0.5f;
            ImU32 alpha = (ImU32)(lcg() * 160 + 80);
            draw->AddCircleFilled(ImVec2(x, y), r, IM_COL32(255, 255, 255, alpha));
        }
    }

    // D. Horizon glow band
    {
        float   horizonY = (float)screenH * 0.62f;
        ImVec2  hBandTop = ImVec2(0, horizonY - 60.0f);
        ImVec2  hBandBottom = ImVec2((float)screenW, horizonY + 60.0f);
        ImU32   glowColor = IM_COL32(220, 140, 60, 60);
        ImU32   glowEdge = IM_COL32(220, 140, 60, 0);

        draw->AddRectFilledMultiColor(hBandTop, hBandBottom,
            glowEdge, glowEdge, glowColor, glowColor);
        draw->AddRectFilledMultiColor(
            ImVec2(0, horizonY), ImVec2((float)screenW, horizonY + 60.0f),
            glowColor, glowColor, glowEdge, glowEdge);
    }


    // E. OS version watermark
    draw->AddText(ImVec2(12.0f, (float)screenH - 28.0f),
        IM_COL32(255, 255, 255, 60), "CSOPESY OS v1.0");

    ImGui::End(); // ##Desktop


    // F. Real-time clock (top-right)
    {
        time_t    now = time(nullptr);
        struct tm localTime;
        localtime_s(&localTime, &now);

        char clockBuf[64];
        strftime(clockBuf, sizeof(clockBuf), "%A, %b %d, %Y  |  %I:%M:%S %p", &localTime);

        ImVec2 clockPad = ImVec2(12.0f, 6.0f);
        ImVec2 textSize = ImGui::CalcTextSize(clockBuf);
        float  clockW = textSize.x + clockPad.x * 2.0f;
        float  clockH = textSize.y + clockPad.y * 2.0f;

        ImGui::SetNextWindowPos(ImVec2((float)screenW - clockW - 10.0f, 8.0f));
        ImGui::SetNextWindowSize(ImVec2(clockW, clockH));
        ImGui::SetNextWindowBgAlpha(0.45f);

        ImGuiWindowFlags clockFlags =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

        ImGui::Begin("##Clock", nullptr, clockFlags);
        ImGui::SetCursorPos(clockPad);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.85f, 1.0f), "%s", clockBuf);
        ImGui::End();
    }

    // G. PWR button (bottom-right) — clean shutdown
    {
        const float btnW = 60.0f;
        const float btnH = 28.0f;
        const float margin = 10.0f;

        ImGui::SetNextWindowPos(ImVec2((float)screenW - btnW - margin,
            (float)screenH - btnH - margin));
        ImGui::SetNextWindowSize(ImVec2(btnW + margin, btnH + margin));
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGuiWindowFlags pwrFlags =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoDecoration;

        ImGui::Begin("##PWR", nullptr, pwrFlags);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.72f, 0.10f, 0.10f, 0.85f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.20f, 0.20f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.05f, 0.05f, 1.00f));

        if (ImGui::Button("PWR", ImVec2(btnW, btnH)))
            shouldClose = true;

        ImGui::PopStyleColor(3);
        ImGui::End();
    }
}


// RenderTaskManager 
static void RenderTaskManager(bool& show)
{
    if (!show) return;

    static std::vector<Process> processes = {
        {1240,  "C+G", "C:\\Program Files\\WindowsApps\\...\\WindowsTerminal.exe", "128 MB"},
        {2344,  "C+G", "D:\\Steam\\bin\\cef\\cef.win64\\steamwebhelper.exe",        "256 MB"},
        {9944,  "C+G", "C:\\Windows\\explorer.exe",                                 "64 MB" },
        {17828, "C+G", "C:\\Program Files\\WindowsApps\\...\\iCloudHome.exe",       "80 MB" },
        {20816, "C+G", "C:\\Users\\User\\AppData\\Roaming\\Spotify\\Spotify.exe",   "312 MB"},
    };

    ImGui::SetNextWindowSize(ImVec2(900, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(150, 120), ImGuiCond_FirstUseEver);

    ImGuiWindowFlags tmFlags = ImGuiWindowFlags_NoCollapse;

    if (ImGui::Begin("Task Manager", &show, tmFlags))
    {
        // Tab bar mimicking Windows Task Manager
        if (ImGui::BeginTabBar("##TMTabs"))
        {
            if (ImGui::BeginTabItem("Processes"))
            {
                // Table with 4 columns
                ImGuiTableFlags tableFlags =
                    ImGuiTableFlags_Borders |
                    ImGuiTableFlags_RowBg |
                    ImGuiTableFlags_ScrollY |
                    ImGuiTableFlags_SizingStretchProp;

                if (ImGui::BeginTable("##ProcTable", 4, tableFlags,
                    ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
                {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableSetupColumn("PID", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                    ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                    ImGui::TableSetupColumn("Memory", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                    ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();

                    for (const auto& p : processes)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%d", p.pid);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextUnformatted(p.status.c_str());
                        ImGui::TableSetColumnIndex(2);
                        ImGui::TextUnformatted(p.memory.c_str());
                        ImGui::TableSetColumnIndex(3);
                        ImGui::TextUnformatted(p.path.c_str());
                    }

                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }

            // Placeholder tabs
            if (ImGui::BeginTabItem("Performance"))
            {
                ImGui::TextDisabled("(placeholder)");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Details"))
            {
                ImGui::TextDisabled("(placeholder)");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
