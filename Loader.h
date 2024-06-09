#pragma once
#include "Util.h"

//should actually do something with this at some point lol
struct ModInfo
{
	std::string ModPath;
	std::wstring ModName;
	std::string ModAuthor = "Unknown Author";
	std::string ModDescription = "No Description";
	std::string ModVersion = "Unknown Version";
	int ModType = 0;
	bool IsEnabled = 0;
	bool WasInitialized = 0;
};

extern std::vector<ModInfo> ModInfoList;

void IndexPaks();
void LoadDlls();