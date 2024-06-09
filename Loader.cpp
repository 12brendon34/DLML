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
			(void)fs::create_directory(modsPath);
			(void)fs::create_directory(subPath);

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
			CurrentMod.ModType = 0;
			CurrentMod.ModPath = entry.path().string();
			ModInfoList.push_back(CurrentMod);
		}
	}
}

void IndexRpaks()
{

}

void LoadDlls()
{
	fs::path asisPath = SetupDir("Asi");

	for (const auto& entry : fs::directory_iterator(asisPath))
	{
		std::string EntryString = entry.path().string();
		auto EntryExtension = str_tolower(entry.path().extension().string());

		if (EntryExtension == ".asi" || EntryExtension == ".dll")
		{
			ModInfo CurrentMod;
			CurrentMod.ModName = L"PlaceHolder";
			CurrentMod.IsEnabled = true;
			CurrentMod.ModType = 2;
			CurrentMod.ModPath = EntryString;
			(void)ModInfoList.push_back(CurrentMod);

			if (LoadLibrary(EntryString.c_str()))
				(void)dbgprintf(">>Plugin loaded: %s\n", EntryString.c_str());
			else
				(void)dbgprintf(">>Plugin error: %s\n", EntryString.c_str());
		}
	}
}