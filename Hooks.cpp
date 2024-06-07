#include "Hooks.h"
#include "global.h"
#include "Loader.h"
#include "Util.h"
#include "kiero/kiero.h"
#include "Menu.h"
#include "impl/d3d11_impl.h"
#include "impl/d3d12_impl.h"
#include "imgui/imgui.h"
#include <fstream>

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







FARPROC OpenPack_Addr; 
FARPROC LoadData_Addr;

//typedef struct SFsFile SFsFile, * PSFsFile;

/*
struct SFsFile {
	buffer<0x0, LONGLONG*> FsFileFileVtable; 	//Length 0x8
	//buffer<0x8, SFsFile*> SFsFile; 				//Length 0x8
};
*/

typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;

struct CResourceDataPack { //fs::mount_path
public:
	union {
		LONGLONG* SFsFile; 						//Length 0x8
		buffer<0x8, UINT> C2;					//Length 0x4
		buffer<0xc, char>C3;					//Length 0x1
		buffer<0xd, char>C4;					//Length 0x1
		buffer<0xe, char>C5;					//Length 0x1
		buffer<0xf, char>C6;					//Length 0x1
		buffer<0x10, LONGLONG*>EventA;			//Length 0x8
		buffer<0x18, UINT>C8;					//Length 0x4  //0 or 192 or smt other bullcrap, it changes, this is used as a flag sometimes, not sure
		buffer<0x1c, undefined4> RpackReference; //Length 0x4 //could be some data, sometimes its just 160,
		buffer<0x20, UINT>C10;					//Length 0x4  //not sure, seen it be data, but ive also seen it just be "00 00 00 A0"
		buffer<0x24, UINT>C11;					//Length 0x1	//seen as 15 or 1
		buffer<0x28, UINT>C15;					//Length 0x4	//seen as 15 or 192
		buffer<0x2c, UINT>C16;					//Length 0x4	//0 or 570433536
		buffer<0x30, UINT>C17;					//Length 0x4	//0 or 1
		buffer<0x34, UINT>C18;					//Length 0x4	//0 or 232
		buffer<0x38, UINT>C19;					//Length 0x4	//0 or 840376832
		buffer<0x3c, UINT>C20;					//Length 0x4	//29 or 1
		buffer<0x40, UINT>C21;					//Length 0x4	//0 or 280
		buffer<0x44, UINT>C22;					//Length 0x4
		buffer<0x48, void*>C23;					//Length 0x8
		buffer<0x50, undefined8>C24;			//Length 0x4					// something interesting but always zero
		buffer<0x58, undefined4>C29;			//Length 0x4	//61 or = lmao
		buffer<0x5c, char>C30;					//Length 0x1	//70 or 57
		buffer<0x5d, char>C31;					//Length 0x1	//01
		buffer<0x5e, char>C32;					//Length 0x1
		buffer<0x5f, char>C33;					//Length 0x1
		buffer<0x60, void*>C34;					//Length 0x8
		buffer<0x68, UINT>C35;					//Length 0x4
		buffer<0x6c, UINT>C36;					//Length 0x4
		buffer<0x70, undefined4>C37;			//Length 0x4	//61 or = lmao
		buffer<0x74, char>C38;					//Length 0x1
		buffer<0x75, char>C39;					//Length 0x1
		buffer<0x76, char>C40;					//Length 0x1
		buffer<0x77, char>C41;					//Length 0x1
		buffer<0x78, void*>C42;					//Length 0x8
		buffer<0x80, UINT>C43;					//Length 0x4
		buffer<0x84, UINT>C44;					//Length 0x4
		buffer<0x88, ULONGLONG>C45;				//Length 0x8	//61 or = lmao
		buffer<0x90, void*>C46;					//Length 0x8
		buffer<0x98, UINT>C47;					//Length 0x4
		buffer<0x9c, UINT>C48;					//Length 0x4
		buffer<0xa0, undefined4>C49;			//Length 0x4	//61 or = lmao
		buffer<0xa4, char>C50;					//Length 0x1
		buffer<0xa5, char>C51;					//Length 0x1
		buffer<0xa6, char>C52;					//Length 0x1
		buffer<0xa7, char>C53;					//Length 0x1
		buffer<0xa8, void*>C54;					//Length 0x8
		buffer<0xb0, UINT>C55;					//Length 0x4
		buffer<0xb4, UINT>C56;					//Length 0x4
		buffer<0xb8, undefined4>C57;			//Length 0x4	//61 or = lmao
		buffer<0xbc, char>C58;					//Length 0x1
		buffer<0xbd, char>C59;					//Length 0x1
		buffer<0xbe, char>C60;					//Length 0x1
		buffer<0xbf, char>C61;					//Length 0x1
		buffer<0xc0, undefined8> PackName;		//Length 0x8
		buffer<0xc8, undefined8> Patch;			//Length 0x8
	};
};

typedef int(__cdecl* t_OpenPack)(CResourceDataPack* othis, char* param_1, UINT param_2);
t_OpenPack o_OpenPack = nullptr;
int hkOpenPack(CResourceDataPack* othis, char* path, UINT flags) {

	(void)dbgprintf("***********\n");
	(void)dbgprintf("OpenPack \n");
	(void)dbgprintf("RpackPath :%s\n", path);
	(void)dbgprintf("flags :%i\n", flags);
	(void)dbgprintf("CResourceDataPack : \n");
	(void)dbgprintf("C8 :%i \n", (UINT)othis->C8);

	int i;
	for (i = 0; i < 208; i++)
	{
		unsigned char c = ((char*)othis)[i];
		(void)dbgprintf("%02x ", c);
	}

	int bruh = o_OpenPack(othis, path, flags);

	(void)dbgprintf("\nOpenPack Result :%i\n", bruh);
	(void)dbgprintf("CResourceDataPack After : \n");
	i = 0;
	for (i = 0; i < 208; i++)
	{
		unsigned char c = ((char*)othis)[i];
		(void)dbgprintf("%02x ", c);
	}

	(void)dbgprintf("\nCResourceDataPack* :%p\n", othis);
	(void)dbgprintf("***********\n\n");

	return bruh;
}

typedef int(__cdecl* t_LoadData)(CResourceDataPack* othis, bool param_1);
t_LoadData o_LoadData = nullptr;
int hkLoadData(CResourceDataPack* othis, bool param_1) {
	int bruh = o_LoadData(othis,  param_1);

	(void)dbgprintf("***********\n");
	(void)dbgprintf("LoadData\n\n");
	(void)dbgprintf("LoadData return: %i\n", bruh);
	(void)dbgprintf("CResourceDataPack* :%p\n", othis);
	(void)dbgprintf("param_1 :%i\n", param_1);
	(void)dbgprintf("***********\n\n");
	return bruh;
}


bool FUN_180026c40(LONGLONG* param_1, byte param_2, byte param_3)
{
	HANDLE pvVar1;
	char extraout_var;

	pvVar1 = CreateEventA(NULL, (UINT)param_2, (UINT)param_3, NULL);
	*param_1 = (LONGLONG)pvVar1;
	return (char)pvVar1 != 0;
}


CResourceDataPack* CResourceDataPackClone() {

	auto alloc = malloc(208); //CResourceDataPack is 208 
	memset(alloc, 0, 208);

	auto resorce = reinterpret_cast<CResourceDataPack*>(alloc);
	
	resorce->C29 = 61;
	resorce->C37 = 61;
	resorce->C45 = 61;
	resorce->C49 = 61;
	resorce->C57 = 61;
	
	LONGLONG* puVar1 = (LONGLONG*)_aligned_malloc(0x30, 0x10);
	*puVar1 = 0;

	puVar1[1] = 0;
	puVar1[2] = 0;
	puVar1[3] = 0;
	puVar1[4] = 0;
	puVar1[5] = 1;
	FUN_180026c40(puVar1 + 3, 1, 0);

	resorce->EventA = puVar1;

	resorce->C2 = 0xfffffff6;

	//resorce->RpackReference = (undefined4)resorce->RpackReference + 1;

	//guessing some varriables
	resorce->RpackReference = 2 ; //not sure how this is done
	resorce->C11 = 15;
	resorce->C15 = 15;
	resorce->C17 = 1;
	resorce->C20 = 1;
	resorce->C31 = 1;

	return resorce;
}

typedef CResourceDataPack* (__cdecl* t_CResourceDataPack)(CResourceDataPack* othis);
t_CResourceDataPack o_CResourceDataPack = nullptr;

CResourceDataPack* hkCResourceDataPack(CResourceDataPack* othis)
{
	return o_CResourceDataPack(othis);
}

void test(char* Path) {

	auto alloc = malloc(208); //CResourceDataPack is 208 
	memset(alloc, 0, 208);

	CResourceDataPack* resorce = hkCResourceDataPack((CResourceDataPack*)alloc);

	//CResourceDataPack* resorce = CResourceDataPackClone();
	
	/*
	bool param_6 = false; // I think this is only used on the engine.dll

	UINT uVar6 = 0x102;
	if (param_6 != false) {
		uVar6 = 0x100;
	}

	if ((uVar6 & 1) != 0) {
		resorce->C8 = resorce->C8 | 0x1000000;
	}
	if ((uVar6 & 8) != 0) {
		resorce->C8 = resorce->C8 | 0x1000000;
	}
	if ((uVar6 & 4) != 0) {
		resorce->C8 = resorce->C8 | 0x4000000;
	}
	*/
	//resorce->Patch = true;

	/*
	0x100
	0x102

	0x100 | 0x200
	0x102 | 0x200
	*/

	auto OpenPacKRet = hkOpenPack(resorce, Path, 0x102 | 0x200);

	if (OpenPacKRet < 0) {
		(void)dbgprintf("OpenPacKRet : %i\n", OpenPacKRet);
		//resorce->PackName = 0;
		//UnloadRpack(resorce);
	}
	auto LoadDataRet = hkLoadData(resorce, true);
	(void)dbgprintf("**********************\n");
	(void)dbgprintf("**********************\n");
	(void)dbgprintf("LoadDataRet : %i\n", LoadDataRet);
	(void)dbgprintf("**********************\n");
	(void)dbgprintf("**********************\n");

	if (LoadDataRet < 0) {
		//FUN_1804002a0(param_1, local_98, pCVar8);
		//resorce->PackName = 0;
		//UnloadRpack(resorce);
		//::CResourceDataPack::ClosePackFiles(pCVar8);
	}
}

typedef LONGLONG(__cdecl* t_BREUHH)(int* param_1);
t_BREUHH o_BREUHH = nullptr;
bool bruhhhhhhhhhhhhhhh = true;

LONGLONG BREUHH(int* param_1) {
	if (bruhhhhhhhhhhhhhhh) {

		test((char*)"E:\\SteamLibrary\\steamapps\\common\\Dying Light\\DW\\Data\\Arr this be the bad blood textures.rpack");
	}

	bruhhhhhhhhhhhhhhh = false;

	return o_BREUHH(param_1);
}







BOOL CreateHooks(HMODULE hmodule) {

	globals.WorkingDir = GetWorkingDir();
	LoadDlls();

	HMODULE EngineDll = GetModuleHandleSimple("engine_x64_rwdi.dll");
	HMODULE FilesystemDll = GetModuleHandleSimple("filesystem_x64_rwdi.dll");

	if (globals.DyingLight2) {
		(void)dbgprintf("DLML2 Loaded\n");

		Fs_Mount_Address = GetProcAddress(FilesystemDll, "?mount@fs@@YA_NAEBUmount_path@1@GPEAPEAVCFsMount@@@Z");
		CResourceLoadingRuntime_Create_Address = GetProcAddress(EngineDll, "?Create@CResourceLoadingRuntime@@SAPEAV1@_N@Z");
		ReadVideoSettings_Addr = *reinterpret_cast<FARPROC>(reinterpret_cast<DWORD64>(EngineDll) + 0x10bdab0);//should eventually do some aob ahh stuff or smt Idn


		(void)HookFunction(Fs_Mount_Address, &hkFs_Mount, reinterpret_cast<void**>(&o_Fs_Mount));
		(void)HookFunction(CResourceLoadingRuntime_Create_Address, &hkCResourceLoadingRuntime_Create, reinterpret_cast<void**>(&o_CResourceLoadingRuntime_Create));

		(void)HookFunction(ReadVideoSettings_Addr, &hkReadVideoSettings, reinterpret_cast<void**>(&o_ReadVideoSettings));
	}
	else {
		(void)dbgprintf("DLML Loaded\n");

		InitializeGameScript_Address = GetProcAddressSimple(EngineDll, "InitializeGameScript");
		Add_Source_Address = GetProcAddressSimple(FilesystemDll, "?add_source@fs@@YA_NPEBDW4ENUM@FFSAddSourceFlags@@@Z");

		(void)HookFunction(InitializeGameScript_Address, &hkInitializeGameScript, reinterpret_cast<void**>(&o_InitializeGameScript));
		(void)HookFunction(Add_Source_Address, &hkAdd_Source, reinterpret_cast<void**>(&o_Add_Source));

		FARPROC CResourceDataPack_Addr = GetProcAddressSimple(EngineDll, "??0CResourceDataPack@@QEAA@XZ");
		(void)HookFunction(CResourceDataPack_Addr, &hkCResourceDataPack, reinterpret_cast<void**>(&o_CResourceDataPack));

		OpenPack_Addr = GetProcAddressSimple(EngineDll, "?OpenPack@CResourceDataPack@@QEAA?AW4ENUM@EResPackErrorCode@@PEBDI@Z");
		(void)HookFunction(OpenPack_Addr, &hkOpenPack, reinterpret_cast<void**>(&o_OpenPack));

		LoadData_Addr = GetProcAddressSimple(EngineDll, "?LoadData@CResourceDataPack@@QEAA?AW4ENUM@EResPackErrorCode@@_N@Z");
		(void)HookFunction(LoadData_Addr, &hkLoadData, reinterpret_cast<void**>(&o_LoadData));

		FARPROC IDN_Addr = *reinterpret_cast<FARPROC>(reinterpret_cast<DWORD64>(EngineDll) + 0x2239a0);
		HookFunction(IDN_Addr, &BREUHH, reinterpret_cast<void**>(&o_BREUHH));
	}

	LogSettingsInstance_Address = GetProcAddress(FilesystemDll, "?Instance@Settings@Log@@SAAEAV12@XZ");
	GetCategoryLevel_Address = GetProcAddress(FilesystemDll, "?GetCategoryLevel@Settings@Log@@QEBA?AW4TYPE@ELevel@2@PEBD@Z");
	CLogV_Address = GetProcAddress(FilesystemDll, "?_CLogV@@YAXW4TYPE@ELevel@Log@@PEBD1HW4ENUM@CLFilterAction@@W44CLLineMode@@1PEAD@Z");

	HookFunction(LogSettingsInstance_Address, &hkLogSettingsInstance, reinterpret_cast<void**>(&o_LogSettingsInstance));
	HookFunction(GetCategoryLevel_Address, &hkGetCategoryLevel, NULL);
	HookFunction(CLogV_Address, &CLogV_Hook, reinterpret_cast<void**>(&CLogV));



	(void)MH_EnableHook(MH_ALL_HOOKS);
	IndexPaks();

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