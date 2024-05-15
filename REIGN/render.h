#pragma once

class Overlay
{
public:
	Overlay(HINSTANCE instance, INT cmd_show, int screenWidth, int screenHeight) noexcept;

	WNDCLASSEXW wc{};
	HWND overlay{};
	RECT client_area{};
	RECT window_area{};
	POINT diff{};
	MARGINS margins{};
	DXGI_SWAP_CHAIN_DESC sd{};
	D3D_FEATURE_LEVEL levels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	ID3D11Texture2D* back_buffer;
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	IDXGISwapChain* swap_chain;
	ID3D11RenderTargetView* render_target_view;
	D3D_FEATURE_LEVEL level;

	int m_screenWidth;
	int m_screenHeight;

	void DrawORect(int x, int y, int w, int h, ImU32 color, int thickness);
	void DrawOCircle(int x, int y, float radius, ImU32 color, int thickness);
	void DrawOLine(float x1, float y1, float x2, float y2, ImU32 color, int thickness);
	void DrawOText(int x, int y, ImU32 color, std::string text);
	void ToggleVisuals(Memory mem, std::uintptr_t client);

	~Overlay();
};