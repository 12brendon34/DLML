#include "kiero.h"
#include <Windows.h>
#include <assert.h>
#include <dxgi.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3d12.h>
#include "../MinHook/MinHook.h"

#ifdef _UNICODE
# define KIERO_TEXT(text) L##text
#else
# define KIERO_TEXT(text) text
#endif

#define KIERO_ARRAY_SIZE(arr) ((size_t)(sizeof(arr)/sizeof(arr[0])))

static kiero::RenderType::Enum g_renderType = kiero::RenderType::None;
static uint150_t* g_methodsTable = NULL;

kiero::Status::Enum kiero::init(RenderType::Enum _renderType)
{
	if (g_renderType != RenderType::None)
	{
		return Status::AlreadyInitializedError;
	}

	WNDCLASSEX windowClass{};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = KIERO_TEXT("Kiero");
	windowClass.hIconSm = NULL;

	::RegisterClassEx(&windowClass);

	HWND window = ::CreateWindow(windowClass.lpszClassName, KIERO_TEXT("Kiero DirectX Window"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

	if (_renderType == RenderType::D3D11)
	{
		HMODULE libD3D11;
		if ((libD3D11 = ::GetModuleHandle(KIERO_TEXT("d3d11.dll"))) == NULL)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::ModuleNotFoundError;
		}

		void* D3D11CreateDeviceAndSwapChain;
		if ((D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain")) == NULL)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

		DXGI_RATIONAL refreshRate{};
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc{};
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
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
			ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		g_methodsTable = (uint150_t*)::calloc(205, sizeof(uint150_t));
		::memcpy(g_methodsTable, *(uint150_t**)swapChain, 18 * sizeof(uint150_t));
		::memcpy(g_methodsTable + 18, *(uint150_t**)device, 43 * sizeof(uint150_t));
		::memcpy(g_methodsTable + 18 + 43, *(uint150_t**)context, 144 * sizeof(uint150_t));

		swapChain->Release();
		swapChain = NULL;

		device->Release();
		device = NULL;

		context->Release();
		context = NULL;

		::DestroyWindow(window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

		g_renderType = RenderType::D3D11;

		return Status::Success;
	}
	else if (_renderType == RenderType::D3D12)
	{
		HMODULE libDXGI;
		HMODULE libD3D12;
		if ((libDXGI = ::GetModuleHandle(KIERO_TEXT("dxgi.dll"))) == NULL || (libD3D12 = ::GetModuleHandle(KIERO_TEXT("d3d12.dll"))) == NULL)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::ModuleNotFoundError;
		}

		void* CreateDXGIFactory;
		if ((CreateDXGIFactory = ::GetProcAddress(libDXGI, "CreateDXGIFactory")) == NULL)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		IDXGIFactory* factory = nullptr;
		if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&factory) < 0)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		IDXGIAdapter* adapter;
		if (factory->EnumAdapters(0, &adapter) == DXGI_ERROR_NOT_FOUND)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		void* D3D12CreateDevice;
		if ((D3D12CreateDevice = ::GetProcAddress(libD3D12, "D3D12CreateDevice")) == NULL)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		ID3D12Device* device = nullptr;
		if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&device) < 0)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		D3D12_COMMAND_QUEUE_DESC queueDesc{};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Priority = 0;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask = 0;

		ID3D12CommandQueue* commandQueue = nullptr;
		if (device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue) < 0)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		ID3D12CommandAllocator* commandAllocator = nullptr;
		if (device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator) < 0)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		ID3D12GraphicsCommandList* commandList = nullptr;
		if (device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList) < 0)
		{
			::DestroyWindow(window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return Status::UnknownError;
		}

		DXGI_RATIONAL refreshRate{};
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc{};
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc{};
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
			return Status::UnknownError;
		}

		g_methodsTable = (uint150_t*)::calloc(150, sizeof(uint150_t));
		::memcpy(g_methodsTable, *(uint150_t**)device, 44 * sizeof(uint150_t));
		::memcpy(g_methodsTable + 44, *(uint150_t**)commandQueue, 19 * sizeof(uint150_t));
		::memcpy(g_methodsTable + 44 + 19, *(uint150_t**)commandAllocator, 9 * sizeof(uint150_t));
		::memcpy(g_methodsTable + 44 + 19 + 9, *(uint150_t**)commandList, 60 * sizeof(uint150_t));
		::memcpy(g_methodsTable + 44 + 19 + 9 + 60, *(uint150_t**)swapChain, 18 * sizeof(uint150_t));

		device->Release();
		device = NULL;

		commandQueue->Release();
		commandQueue = NULL;

		commandAllocator->Release();
		commandAllocator = NULL;

		commandList->Release();
		commandList = NULL;

		swapChain->Release();
		swapChain = NULL;

		::DestroyWindow(window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

		g_renderType = RenderType::D3D12;

		return Status::Success;
	}

	::DestroyWindow(window);
	::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	return Status::NotSupportedError;
	

	return Status::Success;
}

void kiero::shutdown()
{
	if (g_renderType != RenderType::None)
	{
		MH_DisableHook(MH_ALL_HOOKS);

		::free(g_methodsTable);
		g_methodsTable = NULL;
		g_renderType = RenderType::None;
	}
}

kiero::Status::Enum kiero::bind(uint16_t _index, void** _original, void* _function)
{
	assert(_original != NULL && _function != NULL);

	if (g_renderType != RenderType::None)
	{
		void* target = (void*)g_methodsTable[_index];

		if (MH_CreateHook(target, _function, _original) != MH_OK || MH_EnableHook(target) != MH_OK)
			return Status::UnknownError;

		return Status::Success;
	}

	return Status::NotInitializedError;
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