#pragma once
#include <cstdint>
#include <dxgi.h>
#include <d3d11.h>
#include "Menu.h"
#include "MinHook/MinHook.h"
#include <cassert>

#if defined(_M_X64)	
typedef uint64_t uint150_t;
#else
typedef uint32_t uint150_t;
#endif

struct Status
{
	enum Enum
	{
		UnknownError = -1,
		NotSupportedError = -2,
		ModuleNotFoundError = -3,
		Success = 0,
	};
};

Status::Enum bind(uint16_t index, void** original, void* function);
Status::Enum Directx11_Hook();
void Shutdown_Dx();