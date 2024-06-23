#include "win32_impl.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace impl {
	namespace win32 {
		static HWND gHwnd = nullptr;
		static WNDPROC oWndProc = nullptr;
		static HHOOK oMouseProc = nullptr;
		bool is_inactive = false;

		static LRESULT __stdcall hkWindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {

			if (uMsg == WM_KEYUP && wParam == VK_F5)
				Menu::menuToggle = !Menu::menuToggle;

			if (Menu::menuToggle) {
				ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
			}


			if (Menu::menuToggle && !is_inactive)
			{
				CallWindowProc(reinterpret_cast<WNDPROC>(oWndProc), hwnd, WM_ACTIVATE, WA_INACTIVE, 0);
				is_inactive = true;
			}
			else if (!Menu::menuToggle && is_inactive)
			{
				CallWindowProc(reinterpret_cast<WNDPROC>(oWndProc), hwnd, WM_ACTIVATE, WA_ACTIVE, 0);
				is_inactive = false;
			}

			return Menu::menuToggle ? Menu::menuToggle : CallWindowProc(reinterpret_cast<WNDPROC>(oWndProc), hwnd, uMsg, wParam, lParam);
		}

		void init(HWND hwnd) {
			gHwnd = hwnd;
			oWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)hkWindowProc);
		}
	}
}