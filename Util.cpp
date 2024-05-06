#include "Util.h"
#include "global.h"
#include "psapi.h"

fs::path GetWorkingDir() {

	char path_c[MAX_PATH];
	(void)GetModuleFileName(NULL, path_c, MAX_PATH);

	std::string path = std::string(path_c);
	return path.substr(0, path.find_last_of("/\\"));
}

fs::path GetCurrentName(HMODULE hModule) {

	char path_c[MAX_PATH];
	(void)GetModuleFileName(hModule, path_c, MAX_PATH);

	return fs::path(path_c).filename();
}


std::string ToLower(std::string data) {

	(void)std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) { return std::tolower(c); });
	return data;
}