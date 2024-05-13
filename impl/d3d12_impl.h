#include <dxgi1_6.h>
#include <d3d12.h>

#include <exception>

#include "win32_impl.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"

#include "../Menu.h"
#include "../Util.h"

#include "../kiero/kiero.h"

#pragma once
namespace impl {
	namespace d3d12 {
		extern void init();
	}
}