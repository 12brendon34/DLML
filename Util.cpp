#include "Util.h"
#include "global.h"
#include "psapi.h"

fs::path GetWorkingDir() {

	char path_c[MAX_PATH];
	(void)GetModuleFileName(NULL, path_c, MAX_PATH);

	std::string path = std::string(path_c);
	return path.substr(0, path.find_last_of("/\\"));
}

//good enough
bool IsDyingLight() {

	fs::path WDPathName = GetWorkingDir().filename();

	if (WDPathName == "Dying Light")
		return true;
	
	return false;
}