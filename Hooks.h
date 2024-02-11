#pragma once
#include <cstdio>
#include <iostream>
#include <windows.h>
#include "MinHook/MinHook.h"
#include "PakLoader.h"
#include "Menu.h"
#include "Logger.h"

bool __cdecl Add_Source_Hook(char const* Path, int FFSAddSourceFlags);

void __cdecl InitializeGameScript_Hook(LPCSTR param_1);

void __cdecl CLogV_Hook(int logtype, char* thread, char* sourcefile, int linenumber, int CLFilterAction, int CLLineMode, char const* __ptr64 message, char const* __ptr64 printarg);
//annoying ahh

BOOL WINAPI CreateHooks(HMODULE hmodule);