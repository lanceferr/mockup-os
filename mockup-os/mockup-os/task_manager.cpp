#include "task_manager.h"
#include "imgui.h"
#include <vector>

std::string formatProcessName(const std::string& name, size_t max_len) {
    if (name.length() <= max_len) {
        return name;
    }
    return "..." + name.substr(name.length() - (max_len - 3));
}

void renderUI(const std::vector<Process>& processes) {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Task Manager", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Processes");
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
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", proc.pid);
            ImGui::TableSetColumnIndex(1); ImGui::TextUnformatted(proc.type.c_str());
            ImGui::TableSetColumnIndex(2); ImGui::TextUnformatted(formatProcessName(proc.name, 80).c_str());
            ImGui::TableSetColumnIndex(3); ImGui::TextUnformatted(proc.mem_usage.c_str());
        }

        ImGui::EndTable();
    }

    ImGui::End();
}