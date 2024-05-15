// Windows
#include <thread>

// ImGui
#include <dwmapi.h>
#include <d3d11.h>
#include "../External/ImGui/imgui.h"

#include "memory.h"
#include "render.h"

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show)
{
	Memory mem{ "cs2.exe" };
	std::uintptr_t client = mem.GetModuleAddress("client.dll");

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	Overlay overlay(instance, cmd_show, screenWidth, screenHeight);
	overlay.ToggleVisuals(mem, client);

	return 0;
}