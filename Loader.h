#pragma once
#include "Utils/Utils.h"

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

class Loader {
public:
	static auto IndexPaks() -> void;
	static auto SetupDir(const std::string& subfolder) -> std::filesystem::path;
	static std::vector<ModInfo> ModInfoList;
};