#pragma once
#include <cstdio>
#include <iostream>
#include <windows.h>
#include <windows.h>
#include <algorithm>
#include <filesystem>
#include "MinHook/MinHook.h"
#include "global.h"

#ifdef _DEBUG
#  define dbgprintf(...) fprintf(stderr, __VA_ARGS__)
#else
#  define dbgprintf(...) (void)0
#  define _DEBUG 0
#endif

namespace fs = std::filesystem;

fs::path GetWorkingDir();
fs::path GetCurrentName(HMODULE hModule);

std::string str_tolower(std::string s);

void MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...);