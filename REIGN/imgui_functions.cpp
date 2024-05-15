// ImGui
#include <dwmapi.h>
#include <windowsx.h>
#include "../External/ImGui/imgui.h"
#include "../External/ImGui/imgui_impl_dx11.h"
#include "../External/ImGui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param))
		return 0L;

	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0L;
	}

	switch (message)
	{
	case WM_NCHITTEST:
	{
		const LONG borderWidth = GetSystemMetrics(SM_CXSIZEFRAME);
		const LONG titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
		POINT cursorPos = { GET_X_LPARAM(w_param), GET_Y_LPARAM(l_param) };
		RECT windowRect;
		GetWindowRect(window, &windowRect);

		if (cursorPos.y >= windowRect.top && cursorPos.y < windowRect.top + titleBarHeight)
			return HTCAPTION;

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(window, message, w_param, l_param);
}