#include "Hooks.h"
#include "global.h"
#include "Loader.h"

void HookFunction(LPVOID target, LPVOID destination, LPVOID* original) {

	if (!globals.MinHookInitialized && MH_Initialize() == MH_OK) {
		globals.MinHookInitialized = true;
	}

	MH_STATUS status = MH_CreateHook(target, destination, original);
	std::string statusCode = MH_StatusToString(status);

	if (status != MH_OK) {
		printf("Hooked :%p -> :%p\n", target, destination);
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exiting", "Failed to hook %p :%s", target, statusCode);
	}
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
	HMODULE FilesystemDll = GetModuleHandleA("filesystem_x64_rwdi.dll");

	if (EngineDll) {
		printf("Found engine_x64_rwdi.dll BaseAddress at :  %p\n", (void*)EngineDll);
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to create engine_x64_rwdi.dll handle");
	}

	if (FilesystemDll) {
		printf("Found filesystem_x64_rwdi.dll BaseAddress at :  %p\n", (void*)FilesystemDll);
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to create filesystem_x64_rwdi.dll handle");
	}

	#pragma warning(suppress : 6387) //handled, it just doesn't know that
	InitializeGameScript_Address = GetProcAddress(EngineDll, "InitializeGameScript");
	if (InitializeGameScript_Address != NULL) {
		printf("Loaded Libary at :  %p\n", InitializeGameScript_Address);
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exitin g", "Failed");
	}

	#pragma warning(suppress : 6387)
	Add_Source_Address = GetProcAddress(FilesystemDll, "?add_source@fs@@YA_NPEBDW4ENUM@FFSAddSourceFlags@@@Z");
	if (Add_Source_Address != NULL)
	{
		printf("Loaded Libary at :  %p\n", Add_Source_Address);
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exiting", "Failed");
	}

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