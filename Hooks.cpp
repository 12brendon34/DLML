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

#pragma region DyingLight

typedef bool(*add_source)(char const* Path, int FFSAddSourceFlags);
add_source Add_Source_Real;
bool Add_Source(char const* Path, int FFSAddSourceFlags) {
	Menu::AddLog("Added Source : %s, Flags %i\n", Path, FFSAddSourceFlags);
	(void)dbgprintf("Added Source: %s, Flags: %i\n", Path, FFSAddSourceFlags);
	return Add_Source_Real(Path, FFSAddSourceFlags);
}

typedef void(__cdecl* initializegamescript)(LPCSTR locale);
initializegamescript InitializeGameScript_Real;
void InitializeGameScript(LPCSTR locale) {
	for (size_t i = 0; i < ModInfoList.size(); i++)
		if (ModInfoList[i].ModType == 0)
			(void)Add_Source(ModInfoList[i].ModPath.c_str(), 9);

	return InitializeGameScript_Real(locale);
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
_Fs_Mount Fs_Mount = nullptr;

bool Fs_Mount_Hook(fs_mount_path* mount_path, USHORT MountArgs, __int64** param_3) {
	if (mount_path->full_path != NULL) {
		std::string full_path = reinterpret_cast<const char*>(reinterpret_cast<DWORD64>((const char*)mount_path->full_path) & 0x1fffffffffffffff);

		Menu::AddLog("Added Source : %s, Flags %i\n", full_path.c_str(), MountArgs);
		(void)dbgprintf("Added Source: %s, Flags: %i\n", full_path.c_str(), MountArgs);
	}

	return Fs_Mount(mount_path, MountArgs, NULL);
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

	return Fs_Mount_Hook(Path, Args, NULL);
}

typedef bool(__cdecl* _CResourceLoadingRuntime_Create)(bool dontcare);
_CResourceLoadingRuntime_Create CResourceLoadingRuntime_Create = nullptr;

bool CResourceLoadingRuntime_Create_Hook(bool dontcare) {
	for (size_t i = 0; i < ModInfoList.size(); i++)
		if (ModInfoList[i].ModType == 0)
			(void)EasyAdd_Source(ModInfoList[i].ModPath.c_str(), 1);
	return CResourceLoadingRuntime_Create(dontcare);
}
kiero::RenderType::Enum rendererAPI = kiero::RenderType::None;

//"borrowed" from egametools lmao, dw he cool af
typedef bool(__cdecl* t_detourReadVideoSettings)(LPVOID instance, LPVOID file, bool flag1);
t_detourReadVideoSettings o_ReadVideoSettings = nullptr;

static bool hkReadVideoSettings(LPVOID instance, LPVOID file, bool flag1) {
	if (rendererAPI != kiero::RenderType::None)
		return o_ReadVideoSettings(instance, file, flag1);

	DWORD renderer = *reinterpret_cast<PDWORD>(reinterpret_cast<DWORD64>(instance) + 0x7C);
	rendererAPI = !renderer ? kiero::RenderType::D3D11 : kiero::RenderType::D3D12;

	return o_ReadVideoSettings(instance, file, flag1);
}

FARPROC Fs_Mount_Address;
FARPROC CResourceLoadingRuntime_Create_Address;
FARPROC ReadVideoSettings_Addr;
#pragma endregion DyingLight2

BOOL CreateHooks(HMODULE hmodule) {

	globals.WorkingDir = GetWorkingDir();
	IndexPaks();
	LoadDlls();

	HMODULE EngineDll = GetModuleHandleSimple("engine_x64_rwdi.dll");
	HMODULE FilesystemDll = GetModuleHandleSimple("filesystem_x64_rwdi.dll");

	if (globals.DyingLight2) {
		(void)dbgprintf("DLML2 Loaded\n");

		Fs_Mount_Address = GetProcAddress(FilesystemDll, "?mount@fs@@YA_NAEBUmount_path@1@GPEAPEAVCFsMount@@@Z");
		CResourceLoadingRuntime_Create_Address = GetProcAddress(EngineDll, "?Create@CResourceLoadingRuntime@@SAPEAV1@_N@Z");
		ReadVideoSettings_Addr = *reinterpret_cast<FARPROC>(reinterpret_cast<DWORD64>(EngineDll) + 0x10bdab0);//should eventually do some aob ahh stuff


		(void)HookFunction(Fs_Mount_Address, &Fs_Mount_Hook, reinterpret_cast<void**>(&Fs_Mount));
		(void)HookFunction(CResourceLoadingRuntime_Create_Address, &CResourceLoadingRuntime_Create_Hook, reinterpret_cast<void**>(&CResourceLoadingRuntime_Create));

		(void)HookFunction(ReadVideoSettings_Addr, &hkReadVideoSettings, reinterpret_cast<void**>(&o_ReadVideoSettings));
	}
	else {
		(void)dbgprintf("DLML Loaded\n");

		InitializeGameScript_Address = GetProcAddressSimple(EngineDll, "InitializeGameScript");
		Add_Source_Address = GetProcAddressSimple(FilesystemDll, "?add_source@fs@@YA_NPEBDW4ENUM@FFSAddSourceFlags@@@Z");

		(void)HookFunction(InitializeGameScript_Address, &InitializeGameScript, reinterpret_cast<void**>(&InitializeGameScript_Real));
		(void)HookFunction(Add_Source_Address, &Add_Source, reinterpret_cast<void**>(&Add_Source_Real));
	}

	(void)MH_EnableHook(MH_ALL_HOOKS);


	bool init_hook = false;
	do {
		if (!globals.DyingLight2)
			rendererAPI = kiero::RenderType::D3D11;

		if (kiero::init(rendererAPI) != kiero::Status::Success)
			continue;

		if (rendererAPI == kiero::RenderType::D3D11) {
			impl::d3d11::init();
		}
		else {
			impl::d3d12::init();
		}

		init_hook = true;

	} while (!init_hook);

	return true;
}