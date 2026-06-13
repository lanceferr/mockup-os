#include "start_menu.h"
#include <string>

void DrawStartMenu(AppState& st, float sw, float sh) {
    if (!st.showScreen2) return;

    const float tbH = 52.f;
    ImGui::SetNextWindowSize({ 420, 520 }, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos({ 10, sh - tbH - 530 }, ImGuiCond_FirstUseEver);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, HexU32(Pal::WinBg));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, HexU32(0x0D1117));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, HexU32(0x161B22));
    ImGui::PushStyleColor(ImGuiCol_Border, HexU32(Pal::WinBd));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);

    bool open = true;
    if (ImGui::Begin("  File Manager##FM", &open)) {
        ImGui::TextColored(HexCol(Pal::Cyan), "C:\\Users\\student\\Documents");
        ImGui::Separator();
        ImGui::Spacing();

        const char* icon;
        ImU32 color;

        auto FileEntry = [&](const char* name, const char* type, const char* size) {
            if (strcmp(type, "folder") == 0) {
                icon = "[DIR]";
                color = HexU32(Pal::Yellow);
            } else {
                icon = "[FILE]";
                color = HexU32(Pal::TextDim);
            }

            ImGui::PushStyleColor(ImGuiCol_Button, HexU32(0x21262D));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexU32(0x30363D));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexU32(Pal::Accent, 0.3f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
            
            if (ImGui::Button(("##" + std::string(name)).c_str(), { 380, 32 })) {}
            
            ImGui::SameLine(10);
            ImGui::TextColored(HexCol(color), "%s", icon);
            ImGui::SameLine();
            ImGui::Text("%s", name);
            ImGui::SameLine(320);
            ImGui::TextColored(HexCol(Pal::TextDim), "%s", size);
            
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(3);
        };

        FileEntry("Projects", "folder", "");
        FileEntry("Documents", "folder", "");
        FileEntry("Pictures", "folder", "");
        FileEntry("Downloads", "folder", "");
        FileEntry("report.pdf", "file", "2.4 MB");
        FileEntry("notes.txt", "file", "8 KB");
        FileEntry("presentation.pptx", "file", "5.1 MB");
        FileEntry("data.xlsx", "file", "1.2 MB");
        FileEntry("photo.jpg", "file", "3.8 MB");
        FileEntry("music.mp3", "file", "4.5 MB");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(HexCol(Pal::TextDim), "10 items");
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);
    if (!open) st.showScreen2 = false;
}
