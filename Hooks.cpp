#include "Hooks.h"
#include "global.h"
#include "Loader.h"
#include "Util.h"
#include "kiero/kiero.h"
#include "Menu.h"
#include "impl/d3d11_impl.h"
#include "impl/d3d12_impl.h"
#include "imgui/imgui.h"

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

void HookFunction(LPVOID target, LPVOID destination, LPVOID* original, bool enable = false) {

	if (!globals.MinHookInitialized && MH_Initialize() == MH_OK) {
		globals.MinHookInitialized = true;
	}

	MH_STATUS status = MH_CreateHook(target, destination, original);
	std::string statusCode = MH_StatusToString(status);

	if (status == MH_OK) {
		(void)dbgprintf("Hooked %p -> %p\n", target, destination);
	}
	else {
		MsgBoxExit(MB_ICONERROR, "Exiting", "Failed to hook %p : %s", target, statusCode.c_str());
	}
	if (enable)
		(void)MH_EnableHook(target);
}

bool is_initialized = false;

#pragma region Shared
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
	Menu::AddLog("%s", Message.c_str());
	return CLogV(logtype, thread, sourcefile, linenumber, CLFilterAction, CLLineMode, message, printarg);
}


typedef int* (*logsettingsinstance)();
logsettingsinstance o_LogSettingsInstance;
int* hkLogSettingsInstance() {
	int* logint = o_LogSettingsInstance();
	*logint = INT_MAX;
	return logint;
}

int hkGetCategoryLevel(int This, char* Catagory) {
	return INT_MAX;
}


FARPROC LogSettingsInstance_Address;
FARPROC GetCategoryLevel_Address;
FARPROC CLogV_Address;

#pragma endregion Shared

#pragma region DyingLight

typedef bool(*add_source)(char const* Path, int FFSAddSourceFlags);
add_source o_Add_Source;
bool hkAdd_Source(char const* Path, int FFSAddSourceFlags) {
	return o_Add_Source(Path, FFSAddSourceFlags);
}

typedef void(__cdecl* initializegamescript)(LPCSTR locale);
initializegamescript o_InitializeGameScript;
void hkInitializeGameScript(LPCSTR locale) {
	for (size_t i = 0; i < ModInfoList.size(); i++)
		if (ModInfoList[i].ModType == 0)
			(void)hkAdd_Source(ModInfoList[i].ModPath.c_str(), 9);

	return o_InitializeGameScript(locale);
}

FARPROC InitializeGameScript_Address;
FARPROC Add_Source_Address;

#pragma endregion DyingLight

#pragma region DyingLight2

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
//EricPlayZ helped me with the fs_mount_path struct,So I let him copy my dl2 fs_mount hook after I figured out how it works (now I'm out of buisness as his mod is just simply better)

typedef bool(__cdecl* _Fs_Mount)(fs_mount_path* mount_path, USHORT MountArgs, __int64** param_3);
_Fs_Mount o_Fs_Mount = nullptr;

bool hkFs_Mount(fs_mount_path* mount_path, USHORT MountArgs, __int64** param_3) {
	if (mount_path->full_path != NULL) {
		std::string full_path = reinterpret_cast<const char*>(reinterpret_cast<DWORD64>((const char*)mount_path->full_path) & 0x1fffffffffffffff);

		Menu::AddLog("Added Source : %s, Flags %i\n", full_path.c_str(), MountArgs);
		(void)dbgprintf("Added Source: %s, Flags: %i\n", full_path.c_str(), MountArgs);
	}


	return o_Fs_Mount(mount_path, MountArgs, NULL);
}


bool EasyAdd_Source(const char* PakPath, int Args) {

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

	return hkFs_Mount(Path, Args, NULL);
}

typedef bool(__cdecl* _CResourceLoadingRuntime_Create)(bool dontcare);
_CResourceLoadingRuntime_Create o_CResourceLoadingRuntime_Create = nullptr;

bool hkCResourceLoadingRuntime_Create(bool dontcare) {
	for (size_t i = 0; i < ModInfoList.size(); i++)
		if (ModInfoList[i].ModType == 0)
			(void)EasyAdd_Source(ModInfoList[i].ModPath.c_str(), 1);

	return o_CResourceLoadingRuntime_Create(dontcare);
}

typedef bool (*t_IsDx12Enabled)();
t_IsDx12Enabled o_IsDx12Enabled;
bool hkIsDx12Enabled() {
	return o_IsDx12Enabled();
}

typedef bool (*t_LoadResources)(_int64 a);
t_LoadResources o_LoadResources;
bool hkLoadResources(_int64 a) {
	is_initialized = true;
	return o_LoadResources(a);
}

FARPROC Fs_Mount_Address;
FARPROC CResourceLoadingRuntime_Create_Address;
FARPROC LoadResources_Address;
FARPROC IsDx12Enabled_Address;

#pragma endregion DyingLight2

kiero::RenderType::Enum rendererAPI = kiero::RenderType::D3D11;

void HookRenderer() {

	bool init_hook = false;
	do {

		if (globals.DyingLight2) {

			if (!is_initialized)
				continue;

			if (hkIsDx12Enabled())
				rendererAPI = kiero::RenderType::D3D12;
			else 
				rendererAPI = kiero::RenderType::D3D11;

		}

		if (kiero::init(rendererAPI) != kiero::Status::Success)
			continue;


		if (rendererAPI == kiero::RenderType::D3D11)
			impl::d3d11::init();
		else
			impl::d3d12::init();


		init_hook = true;

	} while (!init_hook);
}

BOOL CreateHooks(HMODULE hmodule) {

	globals.WorkingDir = GetWorkingDir();

	LoadDlls();

	HMODULE EngineDll = GetModuleHandleSimple("engine_x64_rwdi.dll");
	HMODULE FilesystemDll = GetModuleHandleSimple("filesystem_x64_rwdi.dll");

	//shared between both engines
	LogSettingsInstance_Address = GetProcAddressSimple(FilesystemDll, "?Instance@Settings@Log@@SAAEAV12@XZ");
	GetCategoryLevel_Address = GetProcAddressSimple(FilesystemDll, "?GetCategoryLevel@Settings@Log@@QEBA?AW4TYPE@ELevel@2@PEBD@Z");
	CLogV_Address = GetProcAddressSimple(FilesystemDll, "?_CLogV@@YAXW4TYPE@ELevel@Log@@PEBD1HW4ENUM@CLFilterAction@@W44CLLineMode@@1PEAD@Z");

	HookFunction(LogSettingsInstance_Address, &hkLogSettingsInstance, reinterpret_cast<void**>(&o_LogSettingsInstance));
	HookFunction(GetCategoryLevel_Address, &hkGetCategoryLevel, NULL);
	HookFunction(CLogV_Address, &CLogV_Hook, reinterpret_cast<void**>(&CLogV));

	if (globals.DyingLight2) {
		(void)dbgprintf("DLML2 Loaded\n");

		LoadResources_Address = GetProcAddressSimple(EngineDll, "?LoadResources@CResourceLoadingRuntime@@QEAA_NXZ");
		IsDx12Enabled_Address = GetProcAddressSimple(FilesystemDll, "?IsDx12Enabled@renderer_status@@YA_NXZ");

		CResourceLoadingRuntime_Create_Address = GetProcAddressSimple(EngineDll, "?Create@CResourceLoadingRuntime@@SAPEAV1@_N@Z");
		Fs_Mount_Address = GetProcAddressSimple(FilesystemDll, "?mount@fs@@YA_NAEBUmount_path@1@GPEAPEAVCFsMount@@@Z");

		(void)HookFunction(LoadResources_Address, &hkLoadResources, reinterpret_cast<void**>(&o_LoadResources), true);
		(void)HookFunction(IsDx12Enabled_Address, &hkIsDx12Enabled, reinterpret_cast<void**>(&o_IsDx12Enabled));

		//rpack loading, CResourceLoadingRuntime::Create is close enough as a replacement for InitializeGameScript (for being used to inject pak's after core paks but before the game loads)
		(void)HookFunction(CResourceLoadingRuntime_Create_Address, &hkCResourceLoadingRuntime_Create, reinterpret_cast<void**>(&o_CResourceLoadingRuntime_Create));
		(void)HookFunction(Fs_Mount_Address, &hkFs_Mount, reinterpret_cast<void**>(&o_Fs_Mount)); //modified add_source
	}
	else {
		(void)dbgprintf("DLML Loaded\n");

		InitializeGameScript_Address = GetProcAddressSimple(EngineDll, "InitializeGameScript");
		Add_Source_Address = GetProcAddressSimple(FilesystemDll, "?add_source@fs@@YA_NPEBDW4ENUM@FFSAddSourceFlags@@@Z");

		(void)HookFunction(InitializeGameScript_Address, &hkInitializeGameScript, reinterpret_cast<void**>(&o_InitializeGameScript));
		(void)HookFunction(Add_Source_Address, &hkAdd_Source, reinterpret_cast<void**>(&o_Add_Source));
	}

	(void)MH_EnableHook(MH_ALL_HOOKS); 

	IndexPaks();
	HookRenderer();

	return true;
}