 #include "PakLoader.h"
#include <filesystem>
#include <libloaderapi.h>

namespace fs = std::filesystem;
std::vector<ModInfo> ModInfoList;

//https://en.cppreference.com/w/cpp/string/byte/tolower too lazy I am, “Powerful you have become, the dark side I sense in you.” – Yoda (I have never watched starwars) 
std::string str_tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return s;
}

void IndexPaks()
{
	//get paks dir
	char path_c[MAX_PATH];
	GetModuleFileNameA(NULL, path_c, MAX_PATH);
	std::string path = std::string(path_c);
	path = path.substr(0, path.find_last_of("/\\"));
	path = path + "\\CustomPaks\\";

	//make if not exist
	ULONGLONG now = GetTickCount64();
	while (!std::filesystem::exists(path))
	{
		if (GetTickCount64() - now > 500)
		{
			std::filesystem::create_directory(path);

			now = GetTickCount64();
		}
	}

	//index
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (str_tolower(entry.path().extension().string()) == ".pak")
		{
			ModInfo CurrentMod;
			CurrentMod.ModName = L"PlaceHolder";
			CurrentMod.IsEnabled = true;
			CurrentMod.ModPath = entry.path().string();
			ModInfoList.push_back(CurrentMod);
		}
	}
}