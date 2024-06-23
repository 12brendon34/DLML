#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cstdint>

#include "../MinHook/MinHook.h"

#ifdef _DEBUG
#  define dbgprintf(...) fprintf(stderr, __VA_ARGS__)
#else
#  define dbgprintf(...) (void)0
#  define _DEBUG 0
#endif

#define Status std::pair<MethodStatus, std::string>


enum class MethodStatus {
	Success = 0,
	Error = 1
	//Failed = 1,
	//Error = 2
};

class Utils {
public:
	static auto MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...) -> void;

	static auto srprintf(LPCSTR sz, ...) ->std::string;

	static auto GetModuleHandleSimple(LPCSTR lpModuleName) -> HMODULE;

	static auto GetProcAddressSimple(HMODULE hModule, LPCSTR lpProcName) -> FARPROC;

	static auto HookFunction(LPVOID target, LPVOID destination, LPVOID* original, const char* name = nullptr, bool enable = false) -> void;

	static auto str_tolower(std::string s) -> std::string;
};