#include "Menu.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool menu_visible = false;
bool is_inactive = false;
bool init = false;

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

LRESULT WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (init)
	{
		if (uMsg == WM_KEYUP && wParam == VK_F5)
			menu_visible = !menu_visible;

		if (menu_visible) {
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		}


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

void InitImGui()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DeltaTime = 1.0f / 60.0f;

	io.MouseDrawCursor = true;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

//modified from imgui_demo
ImGuiTextBuffer     Buf;
ImGuiTextFilter     Filter;
ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
bool                AutoScroll = true;     // Keep scrolling if already at the bottom
bool                Verbose = true;		// Show Full Game Log
bool                ImDemo = false;		// Show Imgui Demo Window

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

void DrawLogWindow(const char* title, bool* p_open = NULL)
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
#ifdef DEBUG
		ImGui::Checkbox("Show Imgui Demo", &ImDemo);
#endif
		ImGui::EndPopup();
	}

	if (ImDemo)
		ImGui::ShowDemoWindow();

	// Main window
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	ImGui::Checkbox("Verbose", &Verbose);
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);


	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear)
		Clear();
	if (copy)
		ImGui::LogToClipboard();

	//Verbose ? MH_EnableHook(GetCategoryLevel_Address) : MH_DisableHook(GetCategoryLevel_Address);
	//Verbose ? MH_EnableHook(LogSettingsInstance_Address) : MH_DisableHook(LogSettingsInstance_Address);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	const char* buf = Buf.begin();
	const char* buf_end = Buf.end();

	if (Filter.IsActive())
	{
		for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
		{
			const char* line_start = buf + LineOffsets[line_no];
			const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
			if (Filter.PassFilter(line_start, line_end))
				ImGui::TextUnformatted(line_start, line_end);
		}
	}
	else
	{
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
	}

	ImGui::PopStyleVar();

	if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();
}

void OnPresent() {

	if (menu_visible)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		DrawLogWindow("Game Log");

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

ID3D11Texture2D* prevBackBuffer;
void SetupRenderTargets(IDXGISwapChain* pSwapchain) {
	(void)dbgprintf("Setting up render targets.\n");
	ID3D11Texture2D* pBackBuffer = nullptr;
	pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
	prevBackBuffer = pBackBuffer;
	pBackBuffer->Release();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer = nullptr;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();

			SetupRenderTargets(pSwapChain);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	OnPresent();

	return oPresent(pSwapChain, SyncInterval, Flags);
}


ResizeBuffers oResizeBuffers;
HRESULT hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{

	(void)dbgprintf("ResizeBuffers called.\n");
	if (mainRenderTargetView) {
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
	}
	HRESULT result = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	if (!pDevice) {
		(void)dbgprintf("ResizeBuffers -> Device did not exist!\n");
	}
	else if (init) {
		SetupRenderTargets(pSwapChain);

		DXGI_SWAP_CHAIN_DESC sd;
		pSwapChain->GetDesc(&sd);
		window = sd.OutputWindow;
		(WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

		(void)dbgprintf("Recreated WndProc.\n");
	}

	return result;
}