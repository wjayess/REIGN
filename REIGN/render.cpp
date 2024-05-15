// Windows
#include <thread>
#include <string_view>
#include <vector>

// ImGui
#include <dwmapi.h>
#include <d3d11.h>
#include "../External/ImGui/imgui_impl_dx11.h"
#include "../External/ImGui/imgui_impl_win32.h"

#include "common.h"
#include "bones.h"
#include "offsets.h"
#include "vector.h"
#include "memory.h"
#include "render.h"

Overlay::Overlay(HINSTANCE instance, INT cmd_show, int screenWidth, int screenHeight) noexcept
{
	device = nullptr;
	device_context = nullptr;
	swap_chain = nullptr;
	render_target_view = nullptr;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = instance;
	wc.lpszClassName = L"hehe";

	RegisterClassExW(&wc);

	overlay = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		L"hehe",
		WS_POPUP,
		0,
		0,
		screenWidth,
		screenHeight,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes(overlay, 0x00FFFFFF, BYTE(255), LWA_ALPHA);

	GetClientRect(overlay, &client_area);
	GetWindowRect(overlay, &window_area);
	ClientToScreen(overlay, &diff);

	margins = {
		window_area.left + (diff.x - window_area.left),
		window_area.top + (diff.y - window_area.top),
		client_area.right,
		client_area.bottom,
	};

	DwmExtendFrameIntoClientArea(overlay, &margins);

	sd.BufferDesc.RefreshRate.Numerator = 166U; // fps
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = overlay;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// create device and that
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&device_context
	);

	back_buffer = { nullptr };
	swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

	if (back_buffer)
	{
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
	}
	else
		return;

	ShowWindow(overlay, cmd_show);
	UpdateWindow(overlay);

	ImGui::CreateContext();
	ImGui::StyleColorsClassic();

	ImGui_ImplWin32_Init(overlay);
	ImGui_ImplDX11_Init(device, device_context);
}

void Overlay::DrawORect(int x, int y, int w, int h, ImU32 color, int thickness)
{
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
}

void Overlay::DrawOCircle(int x, int y, float radius, ImU32 color, int thickness)
{
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, 0, thickness);
}

void Overlay::DrawOLine(float x1, float y1, float x2, float y2, ImU32 color, int thickness)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

void Overlay::DrawOText(int x, int y, ImU32 color, std::string text)
{
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), color, text.c_str());
}

void Overlay::ToggleVisuals(Memory mem, std::uintptr_t client)
{
	ImVec4 bonesColor{255.f, 255.f, 255.f, 75.f};
	ImVec4 healthColor{0.f, 255.0f, 0.f, 75.f};
	ImVec4 nameColor{255.f, 255.f, 255.f, 75.f};

	bool running = true;
	bool toggled = false;
	bool keyDown = false;

	while (running)
	{
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				running = false;
			}
		}

		if (!running)
			break;

		if (GetAsyncKeyState(VK_MENU) < 0 && keyDown == false)
		{
			keyDown = true;

			toggled = !toggled;
		}

		if (GetAsyncKeyState(VK_MENU) == 0 && keyDown == true)
		{
			keyDown = false;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (toggled)
		{
			//uintptr_t localPlayerController = mem.Read<uintptr_t>(client + offset::dwLocalPlayerController); // find local player's controller
			uintptr_t localPlayerPawn = mem.Read<uintptr_t>(client + offset::dwLocalPlayerPawn); // find local player's pawn
			Vector3 localOrigin = mem.Read<Vector3>(localPlayerPawn + offset::m_vOldOrigin); // find local player's origin (feet location)
			view_matrix_t viewMatrix = mem.Read<view_matrix_t>(client + offset::dwViewMatrix); // find location of local player's camera ????
			uintptr_t entityList = mem.Read<uintptr_t>(client + offset::dwEntityList); // find the array of all entities in game
			int localTeam = mem.Read<int>(localPlayerPawn + offset::m_iTeamNum); // find local player's team

			for (int i = 1; i < 32; ++i)
			{
				uintptr_t listEntry = mem.Read<uintptr_t>(entityList + (8 + (i & 0x7FFF) >> 9) + 16); // returns entity at the location relative to i (to loop through)

				if (!listEntry) // not valid? go to the next
					continue;

				uintptr_t playerController = mem.Read<uintptr_t>(listEntry + 120 * (i & 0x1FF)); // find player's corresponding playerController

				if (!playerController) // not a player? go to next
					continue;

				int playerTeam = mem.Read<int>(playerController + offset::m_iTeamNum); // get the player's team

				if (playerTeam == localTeam) // 
					continue;

				uint32_t playerPawn = mem.Read<uint32_t>(playerController + offset::m_hPlayerPawn);

				uintptr_t listEntry2 = mem.Read<uintptr_t>(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);

				if (!listEntry2)
					continue;

				uintptr_t pCSPlayerPawn = mem.Read<uintptr_t>(listEntry2 + 120 * (playerPawn & 0x1FF));

				if (!pCSPlayerPawn)
					continue;

				int health = mem.Read<int>(pCSPlayerPawn + offset::m_iHealth);

				if (health <= 0 || health > 100)
					continue;

				if (pCSPlayerPawn == localPlayerPawn)
					continue;

				int playerSpotted = mem.Read<int>(pCSPlayerPawn + offset::m_entitySpottedState + offset::m_bSpottedByMask);
				int localPlayerSpotted = mem.Read<int>(localPlayerPawn + offset::m_entitySpottedState + offset::m_bSpottedByMask);

				std::string playerName;
				uintptr_t playerNameAddr = mem.Read<uintptr_t>(playerController + offset::m_sSanitizedPlayerName);

				if (playerNameAddr)
				{
					char buffer[MAX_PATH] = {};

					mem.ReadRawMem(playerNameAddr, buffer, MAX_PATH);
					playerName = std::string(buffer);
				}
				else
				{
					playerName = "Unknown";
				}

				uintptr_t gameScene = mem.Read<uintptr_t>(pCSPlayerPawn + offset::m_pGameSceneNode);
				uintptr_t boneArray = mem.Read<uintptr_t>(gameScene + offset::m_modelState + 0x80);

				Vector3 origin = mem.Read<Vector3>(pCSPlayerPawn + offset::m_vOldOrigin);
				Vector3 head = mem.Read<Vector3>(boneArray + bones::head * 32);
				Vector3 feet = origin;

				Vector3 screenPos = origin.WorldToScreen(viewMatrix, m_screenWidth, m_screenHeight);
				Vector3 screenHead = head.WorldToScreen(viewMatrix, m_screenWidth, m_screenHeight);
				Vector3 screenFeet = feet.WorldToScreen(viewMatrix, m_screenWidth, m_screenHeight);

				int boxHeight = screenPos.y - screenHead.y;
				int boxWidth = boxHeight / 2.4f;
				
				ImVec4 boxColor = (playerSpotted & (1 << localPlayerPawn)) ? ImVec4{255.f, 0.f, 0.f, 75.f} : ImVec4{0.f, 0.f, 0.f, 75.f};

				// box
				DrawORect(
					screenHead.x - boxWidth / 2,
					screenHead.y,
					boxWidth,
					boxHeight,
					ImGui::ColorConvertFloat4ToU32(boxColor),
					1.5f
				);

				// head circle
				DrawOCircle(
					screenHead.x,
					screenHead.y,
					5.f,
					ImGui::ColorConvertFloat4ToU32(boxColor),
					1.5f
				);

				// bones
				for (int i = 0; i < sizeof(bConnectionArray) / sizeof(bConnectionArray[0]); i++)
				{
					int bone1 = bConnectionArray[i].bone1;
					int bone2 = bConnectionArray[i].bone2;

					Vector3 VectorBone1 = mem.Read<Vector3>(boneArray + bone1 * 32);
					Vector3 VectorBone2 = mem.Read<Vector3>(boneArray + bone2 * 32);

					Vector3 b1 = VectorBone1.WorldToScreen(viewMatrix, m_screenWidth, m_screenHeight);
					Vector3 b2 = VectorBone2.WorldToScreen(viewMatrix, m_screenWidth, m_screenHeight);

					DrawOLine(b1.x, b1.y, b2.x, b2.y, ImGui::ColorConvertFloat4ToU32(bonesColor), 1.5);
				}

				// name
				DrawOText(
					screenFeet.x,
					screenFeet.y + 15.f,
					ImGui::ColorConvertFloat4ToU32(nameColor),
					playerName
				);

				// health
				DrawOText(
					screenFeet.x,
					screenFeet.y,
					ImGui::ColorConvertFloat4ToU32(healthColor),
					std::to_string(health)
				);
			}
		}

		ImGui::Render();
		float color[4]{ 0, 0, 0, 0 };

		device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
		device_context->ClearRenderTargetView(render_target_view, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swap_chain->Present(0U, 0U);
	}
}

Overlay::~Overlay()
{
	// exiting
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();

	if (swap_chain)
		swap_chain->Release();

	if (device_context)
		device_context->Release();

	if (device)
		device->Release();

	if (render_target_view)
		render_target_view->Release();

	DestroyWindow(overlay);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}