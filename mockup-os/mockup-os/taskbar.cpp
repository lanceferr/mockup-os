#include "taskbar.h"

// ── Helpers ──────────────────────────────────────────────────
ImVec4 HexCol(unsigned hex, float a) {
    return { ((hex >> 16) & 0xFF) / 255.f,
             ((hex >> 8) & 0xFF) / 255.f,
             (hex & 0xFF) / 255.f, a };
}
ImU32 HexU32(unsigned hex, float a) {
    return ImGui::ColorConvertFloat4ToU32(HexCol(hex, a));
}

// ── Taskbar button helper ─────────────────────────────────────
static bool TaskbarButton(const char* label, ImU32 col, ImU32 colHov) {
    ImGui::PushStyleColor(ImGuiCol_Button, HexU32(0x21262D));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colHov);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, col);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
    bool clicked = ImGui::Button(label, { 72, 32 });
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
    return clicked;
}

// ── Taskbar ───────────────────────────────────────────────────
void DrawTaskbar(AppState& st, float sw, float sh) {
    const float tbH = 52.f;
    ImGui::SetNextWindowPos({ 0, sh - tbH }, ImGuiCond_Always);
    ImGui::SetNextWindowSize({ sw, tbH }, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(1.f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, HexU32(Pal::Taskbar));
    ImGui::PushStyleColor(ImGuiCol_Border, HexU32(Pal::TaskbarBd));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10.f, 9.f });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 0.f });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);

    ImGui::Begin("##Taskbar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav);

    ImGui::SameLine();

    if (TaskbarButton("INIT", HexU32(Pal::Accent), HexU32(Pal::AccentHov)))
        st.showScreen1 = !st.showScreen1;
    ImGui::SameLine();

    if (TaskbarButton("START", HexU32(0x1F6FEB), HexU32(Pal::Blue)))
        st.showScreen2 = !st.showScreen2;
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Text, HexU32(Pal::Yellow));
    if (TaskbarButton("TASK", HexU32(0x21262D), HexU32(0x2d2a1a)))
        st.showTaskManager = !st.showTaskManager;
    ImGui::PopStyleColor();
    ImGui::SameLine();

    // Right side: VOL / NET / PWR
    ImGui::SameLine(sw - 200.f);
    ImGui::PushStyleColor(ImGuiCol_Text, HexU32(Pal::TextDim));
    ImGui::Text("VOL"); ImGui::SameLine();
    ImGui::Text("NET"); ImGui::SameLine();
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Text, HexU32(Pal::Red));
    ImGui::PushStyleColor(ImGuiCol_Button, HexU32(0x21262D));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexU32(0x3d1a1a));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexU32(Pal::Red));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
    if (ImGui::Button("PWR", { 48, 32 }))
        st.running = false;
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);

    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

// ── System Info window ────────────────────────────────────────
void DrawSystemInfo(AppState& st, float /*sw*/, float sh) {
    if (!st.showScreen1) return;
    ImGui::SetNextWindowSize({ 480, 380 }, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos({ 80, sh - 480 }, ImGuiCond_FirstUseEver);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, HexU32(Pal::WinBg));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, HexU32(0x0D1117));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, HexU32(0x161B22));
    ImGui::PushStyleColor(ImGuiCol_Border, HexU32(Pal::WinBd));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);

    bool open = true;
    if (ImGui::Begin("  System Information##SI", &open)) {
        ImGui::Dummy({ 0, 4 });
        auto Row = [&](const char* key, const char* val, unsigned vc = Pal::Text) {
            ImGui::TextColored(HexCol(Pal::TextDim), "%-22s", key);
            ImGui::SameLine();
            ImGui::TextColored(HexCol(vc), "%s", val);
            };
        Row("OS", "CSOPESY OS v1.0.0", Pal::Cyan);
        Row("Build", "20260612-x86-x64");
        Row("Kernel", "csopesy-kernel");
        Row("Architecture", "x86_64 (64-bit)");
        ImGui::Separator();
        Row("CPU", "AMD Ryzen 5 5600 6-Core Processor3.5GHz");
        Row("Cores / Threads", "6 Cores / 12 Threads");
        Row("CPU Usage", "23.4 %", Pal::Yellow);
        ImGui::Separator();
        Row("Total RAM", "32 768 MB  (32 GB)");
        Row("Available", "18 432 MB  (18 GB)", Pal::Accent);
        Row("Used", "14 336 MB  (14 GB)");
        ImGui::Separator();
        Row("Disk (C:)", "1TB SSD  –  234 GB free");
        Row("GPU", "NVIDIA GeForce RTX 3070");
        Row("VRAM", "6 GB GDDR6");
        ImGui::Separator();
        Row("Hostname", "CSOPESY-PC");
        Row("User", "student");
        Row("Uptime", "2h 14m 09s");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextColored(HexCol(Pal::TextDim),
            "CSOPESY Desktop OS Emulator  |  Dear ImGui");
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);
    if (!open) st.showScreen1 = false;
}

// ── Global ImGui style ────────────────────────────────────────
void ApplyStyle() {
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 8.f;
    s.FrameRounding = 5.f;
    s.ScrollbarRounding = 6.f;
    s.TabRounding = 6.f;
    s.GrabRounding = 4.f;
    s.WindowBorderSize = 1.f;
    s.FrameBorderSize = 0.f;
    s.ItemSpacing = { 8, 6 };
    s.WindowPadding = { 12, 10 };
    s.FramePadding = { 8, 5 };
    s.ScrollbarSize = 12.f;

    ImVec4* c = s.Colors;
    c[ImGuiCol_Text] = HexCol(Pal::Text);
    c[ImGuiCol_TextDisabled] = HexCol(Pal::TextDim);
    c[ImGuiCol_WindowBg] = HexCol(Pal::WinBg);
    c[ImGuiCol_PopupBg] = HexCol(0x1C2128);
    c[ImGuiCol_Border] = HexCol(Pal::WinBd, 0.8f);
    c[ImGuiCol_FrameBg] = HexCol(0x21262D);
    c[ImGuiCol_FrameBgHovered] = HexCol(0x30363D);
    c[ImGuiCol_FrameBgActive] = HexCol(0x30363D);
    c[ImGuiCol_TitleBg] = HexCol(0x0D1117);
    c[ImGuiCol_TitleBgActive] = HexCol(0x161B22);
    c[ImGuiCol_ScrollbarBg] = HexCol(0x161B22);
    c[ImGuiCol_ScrollbarGrab] = HexCol(0x30363D);
    c[ImGuiCol_ScrollbarGrabHovered] = HexCol(0x484F58);
    c[ImGuiCol_CheckMark] = HexCol(Pal::Accent);
    c[ImGuiCol_SliderGrab] = HexCol(Pal::Accent);
    c[ImGuiCol_Button] = HexCol(0x21262D);
    c[ImGuiCol_ButtonHovered] = HexCol(0x30363D);
    c[ImGuiCol_ButtonActive] = HexCol(Pal::Accent);
    c[ImGuiCol_Header] = HexCol(Pal::Accent, 0.3f);
    c[ImGuiCol_HeaderHovered] = HexCol(Pal::Accent, 0.4f);
    c[ImGuiCol_HeaderActive] = HexCol(Pal::Accent, 0.6f);
    c[ImGuiCol_Separator] = HexCol(Pal::WinBd);
    c[ImGuiCol_Tab] = HexCol(0x21262D);
    c[ImGuiCol_TabHovered] = HexCol(0x30363D);
    c[ImGuiCol_TabActive] = HexCol(Pal::Accent, 0.6f);
    c[ImGuiCol_TabUnfocused] = HexCol(0x21262D);
    c[ImGuiCol_TabUnfocusedActive] = HexCol(0x2D333B);
    c[ImGuiCol_PlotLines] = HexCol(Pal::Cyan);
    c[ImGuiCol_PlotHistogram] = HexCol(Pal::Accent);
    c[ImGuiCol_TableHeaderBg] = HexCol(0x21262D);
    c[ImGuiCol_TableBorderStrong] = HexCol(Pal::WinBd);
    c[ImGuiCol_TableBorderLight] = HexCol(0x21262D);
    c[ImGuiCol_TableRowBg] = HexCol(Pal::WinBg, 0.f);
    c[ImGuiCol_TableRowBgAlt] = HexCol(Pal::RowAlt);
}