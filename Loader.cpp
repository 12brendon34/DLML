#include "Loader.h"

std::vector<ModInfo> Loader::ModInfoList;

auto Loader::SetupDir(const std::string& subfolder) -> std::filesystem::path {
	char path_c[MAX_PATH];

	(void)GetModuleFileNameA(NULL, path_c, MAX_PATH);

	auto path = std::string(path_c);
	std::filesystem::path RootDL = path.substr(0, path.find_last_of("/\\"));

	std::filesystem::path modsPath = RootDL / "Mods";
	std::filesystem::path subPath = modsPath / subfolder;

	ULONGLONG now = GetTickCount64();
	while (!std::filesystem::exists(modsPath) || !std::filesystem::exists(subPath))
	{
		if (GetTickCount64() - now > 500)
		{
			(void)std::filesystem::create_directory(modsPath);
			(void)std::filesystem::create_directory(subPath);

			now = GetTickCount64();
		}
	}

	return subPath;
}

auto Loader::IndexPaks() -> void {
	std::filesystem::path paksPath = SetupDir("Paks");

	for (const auto& entry : std::filesystem::directory_iterator(paksPath))
	{
		if (Utils::str_tolower(entry.path().extension().string()) != ".pak")
			continue;

		ModInfo CurrentMod;
		CurrentMod.ModName = L"PlaceHolder";
		CurrentMod.IsEnabled = true;
		CurrentMod.ModType = 0;
		CurrentMod.ModPath = entry.path().string();
		ModInfoList.push_back(CurrentMod);
	}
}
