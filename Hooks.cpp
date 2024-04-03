#include "Hooks.h"
#include "global.h"

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

typedef bool(*aredataauthenticatedtoplaymultiplayer)();
aredataauthenticatedtoplaymultiplayer AreDataAuthenticatedToPlayMultiplayer_Real;
bool AreDataAuthenticatedToPlayMultiplayer() {
	return true;
}

typedef UINT(*calc_file_crc)(int64_t* CrcCalcArgs);
calc_file_crc Calc_File_Crc_Real;
UINT Calc_File_Crc(int64_t* CrcCalcArgs) {
	return true;
}
/*
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

typedef void(*getmultiplayerfilescrc)();
getmultiplayerfilescrc GetMultiplayerFilesCrc_Real;
void GetMultiplayerFilesCrc() {
	return;
}

typedef int64_t(*createcrcchecker)();
createcrcchecker CreateCrcChecker_Real;
int64_t CreateCrcChecker() {
	int64_t CRC = CreateCrcChecker_Real();
	(void)dbgprintf("CreateCrcChecker Ret: %I64d\n", CRC);
	return CRC;
}
*/

FARPROC AreDataAuthenticatedToPlayMultiplayer_Address;
FARPROC Calc_File_Crc_Address;
/*
FARPROC ILevelReplAddVerifyData_Address;
FARPROC IGameReplAddVerifyData_Address;
FARPROC GetMultiplayerFilesCrc_Address;
FARPROC CreateCrcChecker_Address;
*/

BOOL CreateHooks(HMODULE hmodule) {
	HMODULE EngineDll = GetModuleHandleSimple("engine_x64_rwdi.dll");
	HMODULE FilesystemDll = GetModuleHandleSimple("filesystem_x64_rwdi.dll");
	
	AreDataAuthenticatedToPlayMultiplayer_Address = GetProcAddressSimple(EngineDll, "?AreDataAuthenticatedToPlayMultiplayer@IGame@@QEBA_NXZ");
	Calc_File_Crc_Address = GetProcAddressSimple(FilesystemDll, "?calc_file_crc@fs@@YA_NAEAUCrcCalcArgs@1@@Z");

	HookFunction(AreDataAuthenticatedToPlayMultiplayer_Address, &AreDataAuthenticatedToPlayMultiplayer, reinterpret_cast<void**>(&AreDataAuthenticatedToPlayMultiplayer_Real));
	HookFunction(Calc_File_Crc_Address, &Calc_File_Crc, reinterpret_cast<void**>(&Calc_File_Crc_Real));

	/*
	ILevelReplAddVerifyData_Address = GetProcAddressSimple(EngineDll, "?ReplAddVerifyData@ILevel@@QEAAXPEBDI_N@Z");
	IGameReplAddVerifyData_Address = GetProcAddressSimple(EngineDll, "?ReplAddVerifyData@IGame@@QEAAXPEBDI_N@Z");
	GetMultiplayerFilesCrc_Address = GetProcAddressSimple(EngineDll, "?GetMultiplayerFilesCrc@IGame@@QEBAIXZ");
	CreateCrcChecker_Address = GetProcAddressSimple(EngineDll, "?CreateCrcChecker@CrcCheck@@YAPEAVICrcChecker@1@PEBD00@Z");

	HookFunction(GetMultiplayerFilesCrc_Address, &GetMultiplayerFilesCrc, reinterpret_cast<void**>(&GetMultiplayerFilesCrc_Real));
	HookFunction(ILevelReplAddVerifyData_Address, &ILevelReplAddVerifyData, reinterpret_cast<void**>(&ILevelReplAddVerifyData_Real));
	HookFunction(IGameReplAddVerifyData_Address, &IGameReplAddVerifyData, reinterpret_cast<void**>(&IGameReplAddVerifyData_Real));
	HookFunction(CreateCrcChecker_Address, &CreateCrcChecker, reinterpret_cast<void**>(&CreateCrcChecker_Real));
	*/

	(void)MH_EnableHook(MH_ALL_HOOKS);

	while (globals.Running)
	{
		if (GetAsyncKeyState(VK_DELETE))
			globals.Running = false;

		Sleep(100);
	}

	return true;
}