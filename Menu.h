#pragma once
#include "imgui/imgui.h"

namespace Menu {
	extern void InitImGui();
	extern void FirstTimeRunning(); 

	extern bool menuToggle;
	extern void AddLog(const char* fmt, ...) IM_FMTARGS(2);

	extern void Render();

}
