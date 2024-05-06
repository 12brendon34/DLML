#include "Hooks.h"
#include "global.h"
#include "Loader.h"
#include "Util.h"

typedef unsigned __int64 DWORD64, * PDWORD64;
template<size_t size, typename T> class buffer {
	char buffer[size];
	T data;
public:
	operator T() { return data; }
	T operator->() { return data; }
	T& operator=(const T& other) { data = other; return data; }
	T& operator*=(const T& other) { data *= other; return data; }
	T operator*(const T& other) const { return data * other; }
	T& operator/=(const T& other) { data /= other; return data; }
	T operator/(const T& other) const { return data / other; }
	T& operator+=(const T& other) { data += other; return data; }
	T operator+(const T& other) const { return data + other; }
	T& operator-=(const T& other) { data -= other; return data; }
	T operator-(const T& other) const { return data - other; }
};

struct fs_mount_path { //fs::mount_path
public:
	union {
		const char* root_path;
		buffer<0x8, const char*> unique_tail;
		buffer<0x10, const char*> full_path;
	};
};
//"Borrowed from EricPlayZ on discord" Bc Im Lazy


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


typedef bool(__cdecl* _Fs_Mount)(fs_mount_path* mount_path, USHORT MountArgs, __int64** param_3);
_Fs_Mount Fs_Mount = nullptr;

bool Fs_Mount_Hook(fs_mount_path* mount_path, USHORT MountArgs, __int64** param_3) {
	if (mount_path->full_path != NULL) {
		std::string full_path = reinterpret_cast<const char*>(reinterpret_cast<DWORD64>((const char*)mount_path->full_path) & 0x1fffffffffffffff);
		char buffer[MAX_PATH];

		/*
		std::string root_path = reinterpret_cast<const char*>(reinterpret_cast<DWORD64>((const char*)mount_path->root_path) & 0x1fffffffffffffff);
		std::string unique_tail = reinterpret_cast<const char*>(reinterpret_cast<DWORD64>((const char*)mount_path->unique_tail) & 0x1fffffffffffffff);
		std::string full_path = reinterpret_cast<const char*>(reinterpret_cast<DWORD64>((const char*)mount_path->full_path) & 0x1fffffffffffffff);
		(void)dbgprintf("**************\n");
		(void)dbgprintf("root_path %s\n", (const char*)root_path.c_str());
		(void)dbgprintf("unique_tail %s\n", (const char*)unique_tail.c_str());
		(void)dbgprintf("full_path %s\n", (const char*)full_path.c_str());
		(void)dbgprintf("**************\n");
		*/

		sprintf(buffer, "Added Source : %s, Flags %i\n", full_path.c_str(), MountArgs);
		CLogV_Hook(3, (char*)"DLML2", NULL, NULL, NULL, NULL, buffer, NULL);
	}

	return Fs_Mount(mount_path, MountArgs, NULL);
}



bool Add_Source(const char* PakPath, int Args) {

	//don't care about doing this better, it's fine
	std::string base = PakPath;
	std::string tail = PakPath;
	std::string root = PakPath;

	(void)tail.erase(0, base.find("ph") + 2);
	(void)root.erase(base.find("ph") + 2);


	fs_mount_path* Path = new fs_mount_path;

	Path->root_path = root.c_str();
	Path->unique_tail = tail.c_str();
	Path->full_path = PakPath;

	return Fs_Mount_Hook(Path, Args, NULL);
}

typedef bool(__cdecl* _CResourceLoadingRuntime_Create)(bool dontcare);
_CResourceLoadingRuntime_Create CResourceLoadingRuntime_Create = nullptr;

bool CResourceLoadingRuntime_Create_Hook(bool dontcare) {

	for (size_t i = 0; i < ModInfoList.size(); i++)
		if (ModInfoList[i].IsRpack == false)
			(void)Add_Source(ModInfoList[i].ModPath.c_str(), 1);

	return CResourceLoadingRuntime_Create(dontcare);
}

FARPROC Fs_Mount_Address;
FARPROC CResourceLoadingRuntime_Create_Address;
FARPROC LogSettingsInstance_Address;
FARPROC GetCategoryLevel_Address;
FARPROC CLogV_Address;

BOOL CreateHooks(HMODULE hmodule) {

	globals.WorkingDir = GetWorkingDir();
	IndexPaks();

	HMODULE EngineDll = GetModuleHandleSimple("engine_x64_rwdi.dll");
	HMODULE FilesystemDll = GetModuleHandleSimple("filesystem_x64_rwdi.dll");

	Fs_Mount_Address = GetProcAddress(FilesystemDll, "?mount@fs@@YA_NAEBUmount_path@1@GPEAPEAVCFsMount@@@Z");
	CResourceLoadingRuntime_Create_Address = GetProcAddress(EngineDll, "?Create@CResourceLoadingRuntime@@SAPEAV1@_N@Z");
	LogSettingsInstance_Address = GetProcAddress(FilesystemDll, "?Instance@Settings@Log@@SAAEAV12@XZ");
	GetCategoryLevel_Address = GetProcAddress(FilesystemDll, "?GetCategoryLevel@Settings@Log@@QEBA?AW4TYPE@ELevel@2@PEBD@Z");
	CLogV_Address = GetProcAddress(FilesystemDll, "?_CLogV@@YAXW4TYPE@ELevel@Log@@PEBD1HW4ENUM@CLFilterAction@@W44CLLineMode@@1PEAD@Z");

	HookFunction(Fs_Mount_Address, &Fs_Mount_Hook, reinterpret_cast<void**>(&Fs_Mount));
	HookFunction(CResourceLoadingRuntime_Create_Address, &CResourceLoadingRuntime_Create_Hook, reinterpret_cast<void**>(&CResourceLoadingRuntime_Create));
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