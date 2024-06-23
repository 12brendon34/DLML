#pragma once
#include "Utils/Utils.h"
#include "imgui/imgui.h"
#include "Font.h"

class Menu {
public:
	static bool menuToggle;
public:
	static auto InitImGui() -> void;
	static auto Render() -> void;
	static auto AddLog(const char* fmt, ...) IM_FMTARGS(2) -> void;
	static auto DrawLogWindow(const char* title, bool* p_open = nullptr) -> void;
};