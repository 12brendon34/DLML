#include "Hooks.h"
#include "global.h"

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

typedef UINT(*calc_file_crc)(__int64* CrcCalcArgs);
calc_file_crc Calc_File_Crc_Real;
UINT Calc_File_Crc(__int64* CrcCalcArgs) {
	return true;
}

typedef bool(*aredataauthenticatedtoplaymultiplayer)();
aredataauthenticatedtoplaymultiplayer AreDataAuthenticatedToPlayMultiplayer_Real;
bool AreDataAuthenticatedToPlayMultiplayer() {
	return true;
}

typedef void(*ilevelrepladdverifydata)();
ilevelrepladdverifydata ILevelReplAddVerifyData_Real;
void ILevelReplAddVerifyData() {
	return;
}

typedef void(*igamerepladdverifydata)();
igamerepladdverifydata IGameReplAddVerifyData_Real;
void IGameReplAddVerifyData() {
	return;
}

FARPROC Calc_File_Crc_Address;
FARPROC AreDataAuthenticatedToPlayMultiplayer_Address;
FARPROC ILevelReplAddVerifyData_Address;
FARPROC IGameReplAddVerifyData_Address;

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

	Calc_File_Crc_Address = GetProcAddressSinple(FilesystemDll, "?calc_file_crc@fs@@YA_NAEAUCrcCalcArgs@1@@Z");
	AreDataAuthenticatedToPlayMultiplayer_Address = GetProcAddressSinple(EngineDll, "?AreDataAuthenticatedToPlayMultiplayer@IGame@@QEBA_NXZ");
	ILevelReplAddVerifyData_Address = GetProcAddressSinple(EngineDll, "?ReplAddVerifyData@ILevel@@QEAAXPEBDI_N@Z");
	IGameReplAddVerifyData_Address = GetProcAddressSinple(EngineDll, "?ReplAddVerifyData@IGame@@QEAAXPEBDI_N@Z");

	HookFunction(Calc_File_Crc_Address, &Calc_File_Crc, reinterpret_cast<void**>(&Calc_File_Crc_Real));
	HookFunction(AreDataAuthenticatedToPlayMultiplayer_Address, &AreDataAuthenticatedToPlayMultiplayer, reinterpret_cast<void**>(&AreDataAuthenticatedToPlayMultiplayer_Real));
	HookFunction(ILevelReplAddVerifyData_Address, &ILevelReplAddVerifyData, reinterpret_cast<void**>(&ILevelReplAddVerifyData_Real));
	HookFunction(IGameReplAddVerifyData_Address, &IGameReplAddVerifyData, reinterpret_cast<void**>(&IGameReplAddVerifyData_Real));

	MH_EnableHook(MH_ALL_HOOKS);

	while (globals.Running)
	{
		if (GetAsyncKeyState(VK_DELETE))
			globals.Running = false;

		Sleep(100);
	}

	return true;
}