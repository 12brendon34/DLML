#include "kiero.h"

# include <dxgi.h>
# include <d3d11.h>

# include <dxgi.h>
# include <d3d12.h>

#ifdef _UNICODE
# define KIERO_TEXT(text) L##text
#else
# define KIERO_TEXT(text) text
#endif

#define KIERO_ARRAY_SIZE(arr) ((size_t)(sizeof(arr)/sizeof(arr[0])))

static kiero::RenderType::Enum g_renderType = kiero::RenderType::None;
static uint150_t* g_methodsTable = nullptr;

kiero::status::Enum kiero::init(RenderType::Enum _renderType)
{
	if (g_renderType != RenderType::None)
	{
		return status::AlreadyInitializedError;
	}

	if (_renderType != RenderType::None)
	{
		if (_renderType == RenderType::D3D11)
		{
			WNDCLASSEX windowClass;
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = DefWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = GetModuleHandle(nullptr);
			windowClass.hIcon = nullptr;
			windowClass.hCursor = nullptr;
			windowClass.hbrBackground = nullptr;
			windowClass.lpszMenuName = nullptr;
			windowClass.lpszClassName = KIERO_TEXT("Kiero");
			windowClass.hIconSm = nullptr;

			::RegisterClassEx(&windowClass);

			HWND window = ::CreateWindow(windowClass.lpszClassName, KIERO_TEXT("Kiero DirectX Window"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, windowClass.hInstance, nullptr);

			HMODULE libD3D11;
			if ((libD3D11 = ::GetModuleHandle(KIERO_TEXT("d3d11.dll"))) == nullptr)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::ModuleNotFoundError;
			}

			void* D3D11CreateDeviceAndSwapChain;
			if ((D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain")) == nullptr)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			D3D_FEATURE_LEVEL featureLevel;
			const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

			DXGI_RATIONAL refreshRate;
			refreshRate.Numerator = 60;
			refreshRate.Denominator = 1;

			DXGI_MODE_DESC bufferDesc;
			bufferDesc.Width = 100;
			bufferDesc.Height = 100;
			bufferDesc.RefreshRate = refreshRate;
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			DXGI_SAMPLE_DESC sampleDesc;
			sampleDesc.Count = 1;
			sampleDesc.Quality = 0;

			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			swapChainDesc.BufferDesc = bufferDesc;
			swapChainDesc.SampleDesc = sampleDesc;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.OutputWindow = window;
			swapChainDesc.Windowed = 1;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			IDXGISwapChain* swapChain;
			ID3D11Device* device;
			ID3D11DeviceContext* context;

			if (((long(__stdcall*)(
				IDXGIAdapter*,
				D3D_DRIVER_TYPE,
				HMODULE,
				UINT,
				const D3D_FEATURE_LEVEL*,
				UINT,
				UINT,
				const DXGI_SWAP_CHAIN_DESC*,
				IDXGISwapChain**,
				ID3D11Device**,
				D3D_FEATURE_LEVEL*,
				ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			g_methodsTable = (uint150_t*)::calloc(205, sizeof(uint150_t));
			::memcpy(g_methodsTable, *(uint150_t**)swapChain, 18 * sizeof(uint150_t));
			::memcpy(g_methodsTable + 18, *(uint150_t**)device, 43 * sizeof(uint150_t));
			::memcpy(g_methodsTable + 18 + 43, *(uint150_t**)context, 144 * sizeof(uint150_t));

			swapChain->Release();
			swapChain = nullptr;

			device->Release();
			device = nullptr;

			context->Release();
			context = nullptr;

			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

			g_renderType = RenderType::D3D11;

			return status::Success;
		}
		else if (_renderType == RenderType::D3D12)
		{
			WNDCLASSEX windowClass;
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = DefWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = GetModuleHandle(nullptr);
			windowClass.hIcon = nullptr;
			windowClass.hCursor = nullptr;
			windowClass.hbrBackground = nullptr;
			windowClass.lpszMenuName = nullptr;
			windowClass.lpszClassName = KIERO_TEXT("Kiero");
			windowClass.hIconSm = nullptr;

			::RegisterClassEx(&windowClass);

			HWND window = ::CreateWindow(windowClass.lpszClassName, KIERO_TEXT("Kiero DirectX Window"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, windowClass.hInstance, nullptr);

			HMODULE libDXGI;
			HMODULE libD3D12;
			if ((libDXGI = ::GetModuleHandle(KIERO_TEXT("dxgi.dll"))) == nullptr || (libD3D12 = ::GetModuleHandle(KIERO_TEXT("d3d12.dll"))) == nullptr)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::ModuleNotFoundError;
			}

			void* CreateDXGIFactory;
			if ((CreateDXGIFactory = ::GetProcAddress(libDXGI, "CreateDXGIFactory")) == nullptr)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			IDXGIFactory* factory;
			if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&factory) < 0)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			IDXGIAdapter* adapter;
			if (factory->EnumAdapters(0, &adapter) == DXGI_ERROR_NOT_FOUND)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			void* D3D12CreateDevice;
			if ((D3D12CreateDevice = ::GetProcAddress(libD3D12, "D3D12CreateDevice")) == nullptr)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			ID3D12Device* device;
			if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&device) < 0)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			D3D12_COMMAND_QUEUE_DESC queueDesc;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Priority = 0;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.NodeMask = 0;

			ID3D12CommandQueue* commandQueue;
			if (device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue) < 0)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			ID3D12CommandAllocator* commandAllocator;
			if (device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator) < 0)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			ID3D12GraphicsCommandList* commandList;
			if (device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList) < 0)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			DXGI_RATIONAL refreshRate;
			refreshRate.Numerator = 60;
			refreshRate.Denominator = 1;

			DXGI_MODE_DESC bufferDesc;
			bufferDesc.Width = 100;
			bufferDesc.Height = 100;
			bufferDesc.RefreshRate = refreshRate;
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			DXGI_SAMPLE_DESC sampleDesc;
			sampleDesc.Count = 1;
			sampleDesc.Quality = 0;

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferDesc = bufferDesc;
			swapChainDesc.SampleDesc = sampleDesc;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;
			swapChainDesc.OutputWindow = window;
			swapChainDesc.Windowed = 1;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			IDXGISwapChain* swapChain;
			if (factory->CreateSwapChain(commandQueue, &swapChainDesc, &swapChain) < 0)
			{
				::DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return status::UnknownError;
			}

			g_methodsTable = (uint150_t*)::calloc(172, sizeof(uint150_t));
			::memcpy(g_methodsTable, *(uint150_t**)device, 44 * sizeof(uint150_t));
			::memcpy(g_methodsTable + 44, *(uint150_t**)commandQueue, 19 * sizeof(uint150_t));
			::memcpy(g_methodsTable + 44 + 19, *(uint150_t**)commandAllocator, 9 * sizeof(uint150_t));
			::memcpy(g_methodsTable + 44 + 19 + 9, *(uint150_t**)commandList, 60 * sizeof(uint150_t));
			::memcpy(g_methodsTable + 44 + 19 + 9 + 60, *(uint150_t**)swapChain, 40 * sizeof(uint150_t));

			device->Release();
			device = nullptr;

			commandQueue->Release();
			commandQueue = nullptr;

			commandAllocator->Release();
			commandAllocator = nullptr;

			commandList->Release();
			commandList = nullptr;

			swapChain->Release();
			swapChain = nullptr;

			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

			g_renderType = RenderType::D3D12;

			return status::Success;
		}
	}
	else {
		return status::UnknownError; //RenderType::None, did not succ seed
	}

	return status::Success;
}

void kiero::shutdown()
{
	if (g_renderType != RenderType::None)
	{
		MH_DisableHook(MH_ALL_HOOKS);

		::free(g_methodsTable);
		g_methodsTable = nullptr;
		g_renderType = RenderType::None;
	}
}

kiero::status::Enum kiero::bind(uint16_t _index, void** _original, void* _function)
{
	assert(_original != nullptr && _function != nullptr);


	if (g_renderType != RenderType::None)
	{
		void* target = (void*)g_methodsTable[_index];
		if (MH_CreateHook(target, _function, _original) != MH_OK || MH_EnableHook(target) != MH_OK)
			return status::UnknownError;


		return status::Success;
	}

	return status::NotInitializedError;
}

void kiero::unbind(uint16_t _index)
{
	if (g_renderType != RenderType::None)
		MH_DisableHook((void*)g_methodsTable[_index]);

}

kiero::RenderType::Enum kiero::getRenderType()
{
	return g_renderType;
}

uint150_t* kiero::getMethodsTable()
{
	return g_methodsTable;
}