#include "Menu.h"
#include "Logger.h"
#include "Fonts.h"

ID3D11RenderTargetView* mainRenderTargetView;
ID3D11DeviceContext* pContext;
ResizeBuffers oResizeBuffers;
bool menu_visible = false;
bool is_inactive = false;
ID3D11Device* pDevice;
bool init = false;
Present oPresent;
WNDPROC oWndProc;
HWND window;

void StylishImgui()
{
	auto& style = ImGui::GetStyle();
	style.FrameRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.GrabRounding = 4.0f;
	style.WindowPadding = { 10.f, 10.f };
	style.PopupRounding = 0.f;
	style.FramePadding = { 8.f, 4.f };
	style.ItemSpacing = { 10.f, 8.f };
	style.ItemInnerSpacing = { 6.f, 6.f };
	style.TouchExtraPadding = { 0.f, 0.f };
	style.IndentSpacing = 21.f;
	style.ScrollbarSize = 15.f;
	style.GrabMinSize = 8.f;
	style.ChildBorderSize = 0.f;
	style.FrameBorderSize = 0.f;
	style.TabBorderSize = 0.f;
	style.WindowRounding = 4.f;
	style.ChildRounding = 3.f;
	style.ScrollbarRounding = 4.f;
	style.TabRounding = 4.f;
	style.WindowTitleAlign = { 0.5f, 0.5f };
	style.ButtonTextAlign = { 0.5f, 0.5f };
	style.DisplaySafeAreaPadding = { 3.f, 3.f };

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	colors[ImGuiCol_TextDisabled] = ImColor(42, 42, 42);
	colors[ImGuiCol_WindowBg] = ImColor(18, 12, 27);
	colors[ImGuiCol_ChildBg] = ImColor(18, 12, 27);
	colors[ImGuiCol_PopupBg] = ImColor(18, 12, 27);
	colors[ImGuiCol_Border] = ImColor(35, 32, 40);
	colors[ImGuiCol_BorderShadow] = ImColor(35, 32, 40);
	colors[ImGuiCol_FrameBg] = ImColor(35, 42, 106);
	colors[ImGuiCol_FrameBgHovered] = ImColor(35, 42, 106);
	colors[ImGuiCol_FrameBgActive] = ImColor(34, 122, 180);
	colors[ImGuiCol_TitleBg] = ImColor(39, 51, 125);
	colors[ImGuiCol_TitleBgActive] = ImColor(39, 51, 125);
	colors[ImGuiCol_TitleBgCollapsed] = ImColor(39, 51, 125);
	colors[ImGuiCol_MenuBarBg] = ImColor(39, 51, 125);
	colors[ImGuiCol_ScrollbarBg] = ImColor(18, 12, 27);
	colors[ImGuiCol_ScrollbarGrab] = ImColor(18, 12, 27);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(34, 122, 180);
	colors[ImGuiCol_ScrollbarGrabActive] = ImColor(34, 122, 180);
	colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);
	colors[ImGuiCol_SliderGrab] = ImColor(33, 107, 167);
	colors[ImGuiCol_SliderGrabActive] = ImColor(27, 100, 151);
	colors[ImGuiCol_Button] = ImColor(33, 43, 105);
	colors[ImGuiCol_ButtonHovered] = ImColor(29, 100, 150);
	colors[ImGuiCol_ButtonActive] = ImColor(27, 100, 151);
	colors[ImGuiCol_Header] = ImColor(34, 122, 180);
	colors[ImGuiCol_HeaderHovered] = ImColor(29, 100, 150);
	colors[ImGuiCol_HeaderActive] = ImColor(34, 122, 180);
	colors[ImGuiCol_Separator] = ImColor(46, 46, 46);
	colors[ImGuiCol_SeparatorHovered] = ImColor(46, 46, 46);
	colors[ImGuiCol_SeparatorActive] = ImColor(46, 46, 46);
	colors[ImGuiCol_ResizeGrip] = ImColor(46, 46, 46);
	colors[ImGuiCol_ResizeGripHovered] = ImColor(29, 100, 150);
	colors[ImGuiCol_ResizeGripActive] = ImColor(27, 100, 151);
	colors[ImGuiCol_Tab] = ImColor(33, 43, 105);
	colors[ImGuiCol_TabHovered] = ImColor(34, 122, 180);
	colors[ImGuiCol_TabActive] = ImColor(34, 122, 180);
	colors[ImGuiCol_TabUnfocused] = ImColor(33, 43, 105);
	colors[ImGuiCol_TabUnfocusedActive] = ImColor(34, 122, 180);
}

void InitImGui()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DeltaTime = 1.0f / 60.0f;

	io.MouseDrawCursor = true; //Draw Mouse Cursor bc windows one is offset a bit
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	io.Fonts->AddFontDefault();

	ImFontConfig font_cfg{};
	font_cfg.FontDataOwnedByAtlas = false;
	std::strcpy(font_cfg.Name, "Rubik");

	ImFont* mine = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 20.f, &font_cfg);
	io.FontDefault = mine;

	/* Style */
	StylishImgui();
}

LRESULT WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (init)
	{
		if (uMsg == WM_KEYUP && wParam == VK_F5)
			menu_visible = !menu_visible;

		if (menu_visible) {
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		}

		//marks the window inactive to trick the silly goose into releasing the control of the mouse
		if (menu_visible && !is_inactive)
		{
			CallWindowProc(reinterpret_cast<WNDPROC>(oWndProc), hWnd, WM_ACTIVATE, WA_INACTIVE, 0);
			is_inactive = true;
		}
		else if (!menu_visible && is_inactive)
		{
			CallWindowProc(reinterpret_cast<WNDPROC>(oWndProc), hWnd, WM_ACTIVATE, WA_ACTIVE, 0);
			is_inactive = false;
		}
	}

	return menu_visible ? menu_visible : CallWindowProc(reinterpret_cast<WNDPROC>(oWndProc), hWnd, uMsg, wParam, lParam);
}

ID3D11Texture2D* prevBackBuffer;

void SetupRenderTargets(IDXGISwapChain* pSwapchain) {
	std::cout << "Setting up render targets." << std::endl;
	ID3D11Texture2D* pBackBuffer;
	pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
	prevBackBuffer = pBackBuffer;
	pBackBuffer->Release();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
}

HRESULT hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	Log("ResizeBuffers called.");
	if (mainRenderTargetView) {
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
	}
	HRESULT result = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	if (!pDevice) {

		Log("ResizeBuffers -> Device did not exist!");
	}
	else if (init) {
		SetupRenderTargets(pSwapChain);

		DXGI_SWAP_CHAIN_DESC sd;
		pSwapChain->GetDesc(&sd);
		window = sd.OutputWindow;
		(WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

		Log("Recreated WndProc.");
	}

	return result;
}



ImGuiTextBuffer     Buf;
ImGuiTextFilter     Filter;
ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
bool                AutoScroll = true;     // Keep scrolling if already at the bottom
bool                Verbose = true;		// Show Full Game Log

void Clear()
{
	Buf.clear();
	LineOffsets.clear();
	LineOffsets.push_back(0);
}

void AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
	int old_size = Buf.size();
	va_list args;
	va_start(args, fmt);
	Buf.appendfv(fmt, args);
	va_end(args);
	for (int new_size = Buf.size(); old_size < new_size; old_size++)
		if (Buf[old_size] == '\n')
			LineOffsets.push_back(old_size + 1);
}


void DrawLogWindow(const char* title, bool* p_open)
{
	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}

	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
		ImGui::EndPopup();
	}

	// Main window
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	ImGui::Checkbox("Verbose", &Verbose);

	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear)
		Clear();
	if (copy)
		ImGui::LogToClipboard();

	//too lazy to do this a better way [good]
	Verbose ? MH_EnableHook(GetCategoryLevel_Address) : MH_DisableHook(GetCategoryLevel_Address);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	const char* buf = Buf.begin();
	const char* buf_end = Buf.end();

	ImGuiListClipper clipper;
	clipper.Begin(LineOffsets.Size);
	while (clipper.Step())
	{
		for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
		{
			const char* line_start = buf + LineOffsets[line_no];
			const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
			ImGui::TextUnformatted(line_start, line_end);
		}
	}
	clipper.End();

	ImGui::PopStyleVar();

	if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();
}



HRESULT hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;

			SetupRenderTargets(pSwapChain);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ID3D11Texture2D* curBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&curBackBuffer);
	if (curBackBuffer && prevBackBuffer != curBackBuffer) {
		Log("BackBuffer Changed!");

		if (mainRenderTargetView) {
			mainRenderTargetView->Release();
		}

		pDevice->CreateRenderTargetView(curBackBuffer, NULL, &mainRenderTargetView);
		prevBackBuffer = curBackBuffer;
	}
	curBackBuffer->Release();

	if (menu_visible)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		ImGui::Begin("Game Log");
		ImGui::End();

		DrawLogWindow("Game Log");
		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	return oPresent(pSwapChain, SyncInterval, Flags);
}

