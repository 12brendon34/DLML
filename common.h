#pragma once
#include <cstdio>
#include <iostream>
#include <windows.h>
#include <windows.h>
#include <dsound.h>
#include <algorithm>
#include <filesystem>

#ifdef _DEBUG
#  define dbgprintf(...) fprintf(stderr, __VA_ARGS__)
#else
#  define dbgprintf(...) (void)0
#  define _DEBUG 0
#endif

namespace fs = std::filesystem;

#include "MinHook/MinHook.h"

void MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...);