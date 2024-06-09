﻿#include "d3d11_impl.h"

namespace impl {
	namespace d3d11 {
		ID3D11Device* d3d11Device = nullptr;
		static ID3D11DeviceContext* d3d11DeviceContext = nullptr;
		static ID3D11RenderTargetView* d3d11RenderTargetView = nullptr;

		static std::mutex resourceMutex{};
		static std::vector<IUnknown*> resources{};

		static void ReleaseResources() {
			resourceMutex.lock();
			for (auto resource : resources)
				resource->Release();
			resources.clear();
			d3d11RenderTargetView = nullptr;
			resourceMutex.unlock();
		}
		// Create custom render target view because DX11 color space issue with ImGui bruh
		static ID3D11RenderTargetView* CreateRenderTargetView(IDXGISwapChain* swapChain) {
			ID3D11Device* device = nullptr;
			ID3D11Texture2D* backBuffer = nullptr;
			ID3D11RenderTargetView* renderTarget = nullptr;

			swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));
			swapChain->GetDevice(IID_PPV_ARGS(&device));

			if (device && backBuffer) {
				D3D11_RENDER_TARGET_VIEW_DESC desc{};
				memset(&desc, 0, sizeof(desc));
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

				if (device->CreateRenderTargetView(backBuffer, &desc, &renderTarget) == S_OK) {
					resourceMutex.lock();
					resources.push_back(renderTarget);
					resourceMutex.unlock();
				}
				backBuffer->Release();
			}
			return renderTarget;
		}

		HRESULT(__stdcall* oPresent)(IDXGISwapChain*, UINT, UINT);
		HRESULT __stdcall hkPresent11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
			static bool init = false;

			if (!init) {
				DXGI_SWAP_CHAIN_DESC desc{};
				pSwapChain->GetDesc(&desc);

				pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&d3d11Device);
				d3d11Device->GetImmediateContext(&d3d11DeviceContext);

				d3d11RenderTargetView = CreateRenderTargetView(pSwapChain);

				impl::win32::init(desc.OutputWindow);

				ImGui::CreateContext();
				ImGui::GetIO().IniFilename = nullptr;

				ImGui_ImplWin32_Init(desc.OutputWindow);
				ImGui_ImplDX11_Init(d3d11Device, d3d11DeviceContext);

				Menu::InitImGui();

				init = true;
			}

			for (int retries = 0;; retries++) {
				try {
					ImGui_ImplDX11_NewFrame();
					ImGui_ImplWin32_NewFrame();
					ImGui::NewFrame();

					Menu::FirstTimeRunning();
					if (Menu::menuToggle)
						Menu::Render();

					ImGui::EndFrame();
					ImGui::Render();

					if (d3d11RenderTargetView)
						d3d11DeviceContext->OMSetRenderTargets(1, &d3d11RenderTargetView, nullptr);
					ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

					return oPresent(pSwapChain, SyncInterval, Flags);
				} catch (const std::exception& e) {
					(void)e;
					(void)dbgprintf("Exception thrown rendering ImGui in DX11: {%s}\n", e.what());
					if (retries >= 6) {
						(void)dbgprintf("Retried rendering ImGui in DX11 6 times, game will exit now.\n");
						IM_ASSERT(retries < 6 && "Retried rendering ImGui in DX11 6 times, game will exit now.");
					}
				}
			}
		}

		HRESULT(__stdcall* oResizeBuffers)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
		HRESULT __stdcall hkResizeBuffers11(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
			ReleaseResources();
			HRESULT result = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
			d3d11RenderTargetView = CreateRenderTargetView(pSwapChain);

			return result;
		}

		void init() {
			kiero::Status::Enum ret;

			ret = kiero::bind(8, (LPVOID*)&oPresent, hkPresent11);
			assert(ret == kiero::Status::Success);

			ret = kiero::bind(13, (LPVOID*)&oResizeBuffers, hkResizeBuffers11);
			assert(ret == kiero::Status::Success);
		}
	}
}