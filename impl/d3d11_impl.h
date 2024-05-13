#include <dxgi.h>
#include <d3d11.h>

#include <mutex>
#include <vector>
#include <exception>

#include "win32_impl.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

#include "../Menu.h"
#include "../Util.h"

#include "../kiero/kiero.h"

#pragma once
namespace impl {
	namespace d3d11 {
		extern void init();
	}
}