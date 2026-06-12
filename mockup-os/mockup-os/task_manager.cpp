#include "task_manager.h"      // pulls in palette.h → HexCol, HexU32, Pal
#include "imgui-docking/imgui.h"

std::string formatProcessName(const std::string& name, size_t max_len) {
    if (name.length() <= max_len)
        return name;
    return "..." + name.substr(name.length() - (max_len - 3));
}

void DrawTaskManager(bool& show, float sw, float sh, const std::vector<Process>& processes) {
    if (!show) return;

    ImGui::SetNextWindowSize({ 640, 360 }, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos({ sw * 0.5f - 320.f, sh - 420.f }, ImGuiCond_FirstUseEver);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, HexU32(Pal::WinBg));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, HexU32(0x0D1117));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, HexU32(0x161B22));
    ImGui::PushStyleColor(ImGuiCol_Border, HexU32(Pal::WinBd));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);

    if (ImGui::Begin("  Task Manager##TM", &show)) {
        ImGui::Spacing();
        ImGui::TextColored(HexCol(Pal::TextDim), "Processes");
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::BeginTable("procs", 4,
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingFixedFit,
            ImVec2(0, 0)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("PID", ImGuiTableColumnFlags_WidthFixed, 70);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 50);
            ImGui::TableSetupColumn("Process Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Memory", ImGuiTableColumnFlags_WidthFixed, 80);
            ImGui::TableHeadersRow();

            for (const auto& proc : processes) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored(HexCol(Pal::TextDim), "%d", proc.pid);
                ImGui::TableSetColumnIndex(1);
                ImGui::TextColored(HexCol(Pal::Blue), "%s", proc.type.c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::TextUnformatted(formatProcessName(proc.name, 80).c_str());
                ImGui::TableSetColumnIndex(3);
                ImGui::TextColored(HexCol(Pal::Yellow), "%s", proc.mem_usage.c_str());
            }

            ImGui::EndTable();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);
}