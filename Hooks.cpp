#include "Hooks.h"
#include "global.h"
#include "Loader.h"
#include "Util.h"

HMODULE GetModuleHandleSimple(LPCSTR lpModuleName) {
	HMODULE Handle = GetModuleHandle(lpModuleName);
	if (Handle)
		(void)dbgprintf("Found %s BaseAddress at: %p\n", lpModuleName, Handle);
	else
		MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to create filesystem_x64_rwdi.dll handle");
	return Handle;
}

FARPROC GetProcAddressSimple(HMODULE hModule, LPCSTR lpProcName) {
#pragma warning(suppress : 6387)
	FARPROC Address = GetProcAddress(hModule, lpProcName);
	if (Address)
		(void)dbgprintf("Loaded Libary at: %p\n", Address);
	else
		MsgBoxExit(MB_ICONERROR, "Exiting", "Failed To Get Address");
	return Address;
}

void HookFunction(LPVOID target, LPVOID destination, LPVOID* original) {

	if (!globals.MinHookInitialized && MH_Initialize() == MH_OK) {
		globals.MinHookInitialized = true;
	}

	MH_STATUS status = MH_CreateHook(target, destination, original);
	std::string statusCode = MH_StatusToString(status);

	if (status == MH_OK) {
		(void)dbgprintf("Hooked %p -> %p\n", target, destination);
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exiting", "Failed to hook %p : %s", target, statusCode);
	}
}

typedef bool(*add_source)(char const* Path, int FFSAddSourceFlags);
add_source Add_Source_Real;
bool Add_Source(char const* Path, int FFSAddSourceFlags) {
	return Add_Source_Real(Path, FFSAddSourceFlags);
}

typedef void(__cdecl* initializegamescript)(LPCSTR locale);
initializegamescript InitializeGameScript_Real;
void InitializeGameScript(LPCSTR locale) {
	for (size_t i = 0; i < ModInfoList.size(); i++)
		Add_Source(ModInfoList[i].ModPath.c_str(), 9);

	return InitializeGameScript_Real(locale);
}

FARPROC InitializeGameScript_Address;
FARPROC Add_Source_Address;

BOOL CreateHooks(HMODULE hmodule) {

	globals.WorkingDir = GetWorkingDir();
	IndexPaks();

	HMODULE EngineDll = GetModuleHandleSimple("engine_x64_rwdi.dll");
	HMODULE FilesystemDll = GetModuleHandleSimple("filesystem_x64_rwdi.dll");

	InitializeGameScript_Address = GetProcAddressSimple(EngineDll, "InitializeGameScript");
	Add_Source_Address = GetProcAddressSimple(FilesystemDll, "?add_source@fs@@YA_NPEBDW4ENUM@FFSAddSourceFlags@@@Z");

	//dl2 InitializeGameScript is now LoadGameScriptDll and is not exported
	//InitializeGameScript_Address = (FARPROC)((DWORD_PTR*)EngineDll + 0x7ec400 / (2 * sizeof(DWORD)));

	//dl2 fs::add_source -> fs::mount
	//Add_Source_Address = GetProcAddressSimple(FilesystemDll, "?mount@fs@@YA_NAEBUmount_path@1@GPEAPEAVCFsMount@@@Z");

	HookFunction(InitializeGameScript_Address, &InitializeGameScript, reinterpret_cast<void**>(&InitializeGameScript_Real));
	HookFunction(Add_Source_Address, &Add_Source, reinterpret_cast<void**>(&Add_Source_Real));

	(void)MH_EnableHook(MH_ALL_HOOKS);

	while (globals.Running)
	{
		if (GetAsyncKeyState(VK_DELETE))
			globals.Running = false;

		Sleep(100);
	}

	return true;
}