#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "kiero.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "visuals.h"
#include "math.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

bool menu_open = false;
bool esp_enabled = false;
bool health_esp = false;
bool team_check = true;  // По умолчанию включен (не показывать союзников)

typedef HRESULT(__stdcall* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present oPresent = nullptr;

HRESULT __stdcall hPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    static bool init = false;
    static ID3D11Device* pDevice = nullptr;
    static ID3D11DeviceContext* pContext = nullptr;

    if (!init) {
        pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
        pDevice->GetImmediateContext(&pContext);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(FindWindowA("Valve001", nullptr));  // Окно CS2
        ImGui_ImplDX11_Init(pDevice, pContext);
        init = true;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Тоггл меню на Insert
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        menu_open = !menu_open;
    }

    if (menu_open) {
        ImGui::Begin("CS2 Visuals Menu", &menu_open);
        ImGui::Checkbox("Enable Box ESP", &esp_enabled);
        ImGui::Checkbox("Health ESP", &health_esp);
        ImGui::Checkbox("Team Check (hide teammates)", &team_check);
        ImGui::End();
    }

    // Вызов visuals
    if (esp_enabled) {
        visuals::playerESP();
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(pSwapChain, SyncInterval, Flags);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
            while (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
                Sleep(100);
            }
            kiero::bind(8, (void**)&oPresent, hPresent);  // Хук Present (index 8 для D3D11)
            return 0;
        }, nullptr, 0, nullptr);
    }
    return TRUE;
}
