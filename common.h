#pragma once
#include <cstdio>
#include <iostream>
#include <windows.h>
#include <windows.h>
#include <dsound.h>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

#include "MinHook/MinHook.h"

std::string str_tolower(std::string s);

int MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...);