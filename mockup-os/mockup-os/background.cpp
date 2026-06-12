#include "background.h"

#include "imgui-docking/imgui.h"
#include <ctime>   // Required for the clock

void RenderDesktop(GLFWwindow* window)
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
}