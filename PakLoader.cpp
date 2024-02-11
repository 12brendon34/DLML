 #include "PakLoader.h"
#include <filesystem>
#include <libloaderapi.h>

namespace fs = std::filesystem;
std::vector<ModInfo> ModInfoList;

void IndexPaks()
{
	//get paks dir
	char path_c[MAX_PATH];
	GetModuleFileNameA(NULL, path_c, MAX_PATH);
	std::string path = std::string(path_c);
	path = path.substr(0, path.find_last_of("/\\"));
	path = path + "\\Mods\\Paks\\";

	//make if not exist
	if (!std::filesystem::exists(path))
	{
		std::filesystem::create_directory(path);
	}

	//index
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.path().extension().string() == ".pak")
		{
			ModInfo CurrentMod;
			CurrentMod.ModName = L"PlaceHolder";
			CurrentMod.IsEnabled = true;
			CurrentMod.ModPath = entry.path().string();
			ModInfoList.push_back(CurrentMod);
		}
	}
}