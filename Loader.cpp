#include "Loader.h"

std::vector<ModInfo> ModInfoList;
char path_c[MAX_PATH];

fs::path SetupDir(std::string subfolder) {
	(void)GetModuleFileNameA(NULL, path_c, MAX_PATH);

	std::string path = std::string(path_c);
	fs::path RootDL = path.substr(0, path.find_last_of("/\\"));

	fs::path modsPath = RootDL / "Mods";
	fs::path subPath = modsPath / subfolder;

	std::cout << modsPath << std::endl;
	std::cout << subPath << std::endl;

	ULONGLONG now = GetTickCount64();
	while (!fs::exists(modsPath) || !fs::exists(subPath))
	{
		if (GetTickCount64() - now > 500)
		{
			fs::create_directory(modsPath);
			fs::create_directory(subPath);

			now = GetTickCount64();
		}
	}

	return subPath;
}

void IndexPaks()
{
	fs::path paksPath = SetupDir("Paks");

	for (const auto& entry : fs::directory_iterator(paksPath))
	{
		if (str_tolower(entry.path().extension().string()) == ".pak")
		{
			ModInfo CurrentMod;
			CurrentMod.ModName = L"PlaceHolder";
			CurrentMod.IsEnabled = true;
			CurrentMod.IsRpack = false;
			CurrentMod.ModPath = entry.path().string();
			ModInfoList.push_back(CurrentMod);
		}
	}
}

void IndexRpaks()
{

}

void IndexDlls()
{

}