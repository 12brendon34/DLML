#pragma once
#include "common.h"

fs::path GetWorkingDir();
fs::path GetCurrentName(HMODULE hModule);
std::string ToLower(std::string data);
bool IsDyingLight();