#pragma once

#include "imgui-docking/imgui.h"

// ── Helpers (defined once in taskbar.cpp) ────────────────────
ImVec4 HexCol(unsigned hex, float a = 1.f);
ImU32  HexU32(unsigned hex, float a = 1.f);

// ── Colour palette ───────────────────────────────────────────
namespace Pal {
    constexpr unsigned BG = 0x0D1117;
    constexpr unsigned Taskbar = 0x161B22;
    constexpr unsigned TaskbarBd = 0x30363D;
    constexpr unsigned Accent = 0x238636;
    constexpr unsigned AccentHov = 0x2EA043;
    constexpr unsigned Red = 0xDA3633;
    constexpr unsigned Yellow = 0xE3B341;
    constexpr unsigned Blue = 0x388BFD;
    constexpr unsigned Cyan = 0x39D353;
    constexpr unsigned Text = 0xE6EDF3;
    constexpr unsigned TextDim = 0x8B949E;
    constexpr unsigned WinBg = 0x161B22;
    constexpr unsigned WinBd = 0x30363D;
    constexpr unsigned RowAlt = 0x1C2128;
}

// ── App state ─────────────────────────────────────────────────
struct AppState {
    bool  showTaskManager = false;
    bool  showScreen1 = false;   // System Info
    bool  showScreen2 = false;   // File Explorer
    bool  running = true;
    float time = 0.f;
    int   sortCol = 2;
    bool  sortAsc = false;
};