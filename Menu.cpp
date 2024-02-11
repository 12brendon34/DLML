#include "Menu.h"
#include "Logger.h"

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

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = true; //Draw Mouse Cursor bc windows one is offset a bit
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
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

	/*
	if (mainRenderTargetView)
	{
		pContext->OMSetRenderTargets(0, NULL, NULL);
		mainRenderTargetView->Release();
		mainRenderTargetView = NULL;
	}

	HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	if (SUCCEEDED(hr))
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();
		

		HRESULT hr = S_OK;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;

		hr = pSwapChain->GetDesc(&SwapChainDesc);

		if (FAILED(hr))
		{
			ERROR("Failed to get swapchain description");
			return hr;
		}

		// Create the render target
		ID3D11Texture2D* pBackBuffer;
		D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;

		ZeroMemory(&RenderTargetViewDesc, sizeof(RenderTargetViewDesc));

		RenderTargetViewDesc.Format = SwapChainDesc.BufferDesc.Format;
		RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;


		ID3D11Texture2D* pBuffer;
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

		if (FAILED(hr))
		{
			ERROR("Failed to get backbuffer from swapchain");
			return hr;
		}

		if (pBuffer)
		{
			hr = pDevice->CreateRenderTargetView(pBuffer, &RenderTargetViewDesc, &mainRenderTargetView);

			if (SUCCEEDED(hr))
				pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
			else
				ERROR("Failed to CreateRenderTargetView for backbuffer");

			D3D11_VIEWPORT vp;
			vp.Width = Width;
			vp.Height = Height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			pContext->RSSetViewports(1, &vp);

			pBuffer->Release();
		}

	}
	return hr;
	*/
}



ImGuiTextBuffer     Buf;
ImGuiTextFilter     Filter;
ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
bool                AutoScroll = true;     // Keep scrolling if already at the bottom
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
		ImGui::Checkbox("Show Imgui Demo", &ImDemo);
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

	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear)
		Clear();
	if (copy)
		ImGui::LogToClipboard();

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
		
		ImGui::SetNextWindowSize(ImVec2(700, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("Game Log");
		ImGui::End();

		DrawLogWindow("Game Log");
		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	return oPresent(pSwapChain, SyncInterval, Flags);
}

