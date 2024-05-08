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

//forgot I had common.cpp
//https://en.cppreference.com/w/cpp/string/byte/tolower bit lazy 
std::string str_tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return s;
}

void MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...)
{
	char ach[512];
	va_list args;
	va_start(args, sz);
	(void)wvsprintf(ach, sz, args);
	(void)MessageBox(NULL, ach, lpCaption, nType);
	ExitProcess(0);
}