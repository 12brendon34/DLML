#include "Hooks.h"
#include "global.h"
#include "Loader.h"

void HookFunction(LPVOID target, LPVOID destination, LPVOID* original) {

	if (!globals.MinHookInitialized && MH_Initialize() == MH_OK) {
		globals.MinHookInitialized = true;
	}

	MH_STATUS status = MH_CreateHook(target, destination, original);
	std::string statusCode = MH_StatusToString(status);

	if (status == MH_OK) {
#ifdef _DEBUG
		printf("Hooked %p -> %p\n", target, destination);
#endif
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exiting", "Failed to hook %p : %s", target, statusCode);
	}
}

FARPROC GetProcAddressSinple(HMODULE hModule, LPCSTR lpProcName) {
#pragma warning(suppress : 6387)
	FARPROC Source_Address = GetProcAddress(hModule, lpProcName);
	if (Source_Address != NULL)
	{
#ifdef _DEBUG
		printf("Loaded Libary at: %p\n", Source_Address);
#endif
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exiting", "Failed");
	}
	return Source_Address;
}

typedef bool(*add_source)(char const* Path, int FFSAddSourceFlags);
add_source Add_Source_Real;
bool Add_Source(char const* Path, int FFSAddSourceFlags) {
	return Add_Source_Real(Path, FFSAddSourceFlags);
}

typedef void(__cdecl* initializegamescript)(LPCSTR locale);
initializegamescript InitializeGameScript_Real = nullptr;
void InitializeGameScript(LPCSTR locale) {
	for (size_t i = 0; i < ModInfoList.size(); i++)
		Add_Source(ModInfoList[i].ModPath.c_str(), 9);

	return InitializeGameScript_Real(locale);
}

FARPROC InitializeGameScript_Address;
FARPROC Add_Source_Address;

BOOL CreateHooks(HMODULE hmodule) {

	HMODULE EngineDll = GetModuleHandleA("engine_x64_rwdi.dll");
	if (EngineDll) {
#ifdef _DEBUG
		printf("Found engine_x64_rwdi.dll BaseAddress at: %p\n", EngineDll);
#endif
	}
	else
		MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to create engine_x64_rwdi.dll handle");

	HMODULE FilesystemDll = GetModuleHandleA("filesystem_x64_rwdi.dll");
	if (FilesystemDll) {
#ifdef _DEBUG
		printf("Found filesystem_x64_rwdi.dll BaseAddress at: %p\n", FilesystemDll);
#endif

	}
	else
		MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to create filesystem_x64_rwdi.dll handle");


	InitializeGameScript_Address = GetProcAddressSinple(EngineDll, "InitializeGameScript");
	Add_Source_Address = GetProcAddressSinple(FilesystemDll, "?add_source@fs@@YA_NPEBDW4ENUM@FFSAddSourceFlags@@@Z");

	HookFunction(InitializeGameScript_Address, &InitializeGameScript, reinterpret_cast<void**>(&InitializeGameScript_Real));
	HookFunction(Add_Source_Address, &Add_Source, reinterpret_cast<void**>(&Add_Source_Real));

	IndexPaks();
	MH_EnableHook(MH_ALL_HOOKS);

	while (globals.Running)
	{
		if (GetAsyncKeyState(VK_DELETE))
			globals.Running = false;

		Sleep(100);
	}

	return true;
}