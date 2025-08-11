#include "visuals.h"
#include "imgui.h"
#include "math.h"
#include <windows.h>
#include <cstdio>  // Для sprintf_s

// Актуальные offsets на август 2025 (client.dll)
constexpr uintptr_t dwEntityList = 0x197E7C8;
constexpr uintptr_t dwLocalPlayerPawn = 0x17D9B38;
constexpr uintptr_t m_iHealth = 0x334;
constexpr uintptr_t m_iTeamNum = 0x3CB;
constexpr uintptr_t m_vecOrigin = 0x127C;
constexpr uintptr_t dwViewMatrix = 0x19DE080;

bool esp_enabled = false;
bool health_esp = false;
bool team_check = true;

void visuals::playerESP() {
    uintptr_t moduleBase = (uintptr_t)GetModuleHandleA("client.dll");
    if (!moduleBase) return;

    float viewMatrix[16];
    memcpy(viewMatrix, (PBYTE*)(moduleBase + dwViewMatrix), sizeof(viewMatrix));

    uintptr_t localPlayer = *(uintptr_t*)(moduleBase + dwLocalPlayerPawn);
    if (!localPlayer) return;
    int localTeam = *(int*)(localPlayer + m_iTeamNum);

    uintptr_t entityList = *(uintptr_t*)(moduleBase + dwEntityList);
    if (!entityList) return;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    for (int i = 1; i < 64; ++i) {  // До 64 entity
        uintptr_t listEntry = *(uintptr_t*)(entityList + 0x8 * (i >> 9) + 0x10);
        if (!listEntry) continue;
        uintptr_t player = *(uintptr_t*)(listEntry + 120 * (i & 0x1FF));
        if (!player || player == localPlayer) continue;

        int playerTeam = *(int*)(player + m_iTeamNum);
        if (team_check && playerTeam == localTeam) continue;  // Team Check

        int health = *(int*)(player + m_iHealth);
        if (health < 1 || health > 100) continue;

        Vec3 pos = *(Vec3*)(player + m_vecOrigin);
        Vec2 screenPos;
        if (!worldToScreen(pos, screenPos, viewMatrix, screenWidth, screenHeight)) continue;

        // Box ESP: Простой прямоугольник (можно улучшить на bounding box)
        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(screenPos.x - 20, screenPos.y - 40), ImVec2(screenPos.x + 20, screenPos.y + 40), IM_COL32(255, 0, 0, 255), 1.0f);  // Красный бокс

        // Health ESP
        if (health_esp) {
            char healthText[16];
            sprintf_s(healthText, "%d HP", health);
            ImGui::GetBackgroundDrawList()->AddText(ImVec2(screenPos.x - 20, screenPos.y + 45), IM_COL32(255, 255, 255, 255), healthText);
        }
    }
}