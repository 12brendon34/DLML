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

typedef int* (*logsettingsinstance)();
logsettingsinstance LogSettingsInstance;
int* LogSettingsInstance_Hook() {
	int* logint = LogSettingsInstance();
	*logint = INT_MAX;
	return logint;
}

typedef int(*getcategorylevel)(int This, char* Catagory);
getcategorylevel GetCategoryLevel;
int GetCategoryLevel_Hook(int This, char* Catagory) { //GetCategoryLevel
	return INT_MAX;
}

typedef void(__cdecl* _clogv)(int LogType, char* thread, char* sourcefile, int linenumber, int CLFilterAction, int CLLineMode, char const* __ptr64 message, char const* __ptr64 printarg);
_clogv CLogV = nullptr;
void CLogV_Hook(int logtype, char* thread, char* sourcefile, int linenumber, int CLFilterAction, int CLLineMode, char const* __ptr64 message, char const* __ptr64 printarg) {
	std::string Message;

	if (logtype == 5)
		Message.append(" DBUG :");
	else if (logtype == 4)
		Message.append(" WARN :");
	else if (logtype == 3)
		Message.append(" INFO :");
	else if (logtype == 2)
		Message.append(" ERRR :");

	Message.append(" [" + (std::string)thread + "] ");

	if (CLFilterAction == 2)
		Message.append("| ");
	else
		Message.append("> ");

	char Buffer[65528];
	vsprintf(Buffer, message, (va_list)printarg);
	Message.append(Buffer);

	(void)dbgprintf("%s", Message.c_str());
	return CLogV(logtype, thread, sourcefile, linenumber, CLFilterAction, CLLineMode, message, printarg);
}

FARPROC LogSettingsInstance_Address;
FARPROC GetCategoryLevel_Address;
FARPROC CLogV_Address;

BOOL CreateHooks(HMODULE hmodule) {

	globals.WorkingDir = GetWorkingDir();
	IndexPaks();

	HMODULE EngineDll = GetModuleHandleSimple("engine_x64_rwdi.dll");
	HMODULE FilesystemDll = GetModuleHandleSimple("filesystem_x64_rwdi.dll");

	//dl2 InitializeGameScript is now LoadGameScriptDll and is not exported
	//InitializeGameScript_Address = (FARPROC)((DWORD_PTR*)EngineDll + 0x7ec400 / (2 * sizeof(DWORD)));

	//dl2 fs::add_source -> fs::mount

	LogSettingsInstance_Address = GetProcAddress(FilesystemDll, "?Instance@Settings@Log@@SAAEAV12@XZ");
	GetCategoryLevel_Address = GetProcAddress(FilesystemDll, "?GetCategoryLevel@Settings@Log@@QEBA?AW4TYPE@ELevel@2@PEBD@Z");
	CLogV_Address = GetProcAddress(FilesystemDll, "?_CLogV@@YAXW4TYPE@ELevel@Log@@PEBD1HW4ENUM@CLFilterAction@@W44CLLineMode@@1PEAD@Z");

	HookFunction(LogSettingsInstance_Address, &LogSettingsInstance_Hook, reinterpret_cast<void**>(&LogSettingsInstance));
	HookFunction(GetCategoryLevel_Address, &GetCategoryLevel_Hook, reinterpret_cast<void**>(&GetCategoryLevel));
	HookFunction(CLogV_Address, &CLogV_Hook, reinterpret_cast<void**>(&CLogV));

	(void)MH_EnableHook(MH_ALL_HOOKS);

	while (globals.Running)
	{
		if (GetAsyncKeyState(VK_DELETE))
			globals.Running = false;

		Sleep(100);
	}

	return true;
}