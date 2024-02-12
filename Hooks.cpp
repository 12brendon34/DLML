#include "Hooks.h"
//#include "TestHook.h"

typedef bool(*add_source)(char const* __ptr64, int FFSAddSourceFlags);
add_source Add_Source;
bool Add_Source_Hook(char const* Path, int FFSAddSourceFlags) {
	bool Status = Add_Source(Path, FFSAddSourceFlags);

	if (Status == true) {
		std::string message = "Added Source: " + (std::string)Path + " SourceFlag: " + std::to_string(FFSAddSourceFlags) + "\n";
		Log(message);
	}

	return Status;
}

typedef void(__cdecl* initializegamescript)(LPCSTR param_1);
initializegamescript InitializeGameScript = nullptr;
void InitializeGameScript_Hook(LPCSTR param_1) {

	for (size_t i = 0; i < ModInfoList.size(); i++)
		Add_Source_Hook(ModInfoList[i].ModPath.c_str(), 9);
	Log((std::string)"Initializing Game Scripts\n");

	return InitializeGameScript(param_1);
}

typedef void(__cdecl* _clogv)(LogType LogType, char* thread, char* sourcefile, int linenumber, int CLFilterAction, int CLLineMode, char const* __ptr64 message, char const* __ptr64 printarg);
_clogv CLogV = nullptr;
void CLogV_Hook(int logtype, char* thread, char* sourcefile, int linenumber, int CLFilterAction, int CLLineMode, char const* __ptr64 message, char const* __ptr64 printarg) {
	std::string Message;

	//this is a really bad rn

	if (logtype == LogType::DEBG)
		Message.append(" DBUG :");
	else if (logtype == LogType::WARN)
		Message.append(" WARN :");
	else if (logtype == LogType::INFO)
		Message.append(" INFO :");
	else if (logtype == LogType::ERRR)
		Message.append(" ERRR :");

	Message.append(" [" + (std::string)thread + "] ");

#ifdef DEBUG
	printf(">%s(%d): ", sourcefile, linenumber);
#endif
	if (CLFilterAction == 2)
		Message.append("| ");
	else
		Message.append("> ");

	char Buffer[65528];
	vsprintf(Buffer, message, (va_list)printarg);
	Message.append(Buffer);
	AddLog(Message.c_str());
	return CLogV((LogType)logtype, thread, sourcefile, linenumber, CLFilterAction, CLLineMode, message, printarg);
}

typedef int(*getcategorylevel)(int This, char* Catagory);
getcategorylevel GetCategoryLevel;
int GetCategoryLevel_Hook(int This, char* Catagory) { //GetCategoryLevel
	//Log(param_1);
	//int Status = Function(NULL, param_1);
	return 255;
}

//whatever
FARPROC InitializeGameScript_Address;
FARPROC Add_Source_Address;
FARPROC CLogV_Address;
FARPROC GetCategoryLevel_Address;

BOOL CreateHooks(HMODULE hmodule) {
	HMODULE EngineDll = GetModuleHandleA("engine_x64_rwdi.dll");
	HMODULE FilesystemDll = GetModuleHandleA("filesystem_x64_rwdi.dll");

	AddLog("Found engine_x64_rwdi.dll BaseAddress at :  %p\n", (void*)EngineDll);
	AddLog("Found filesystem_x64_rwdi.dll BaseAddress at :  %p\n", (void*)FilesystemDll);

	InitializeGameScript_Address = GetProcAddress(EngineDll, "InitializeGameScript");
	Add_Source_Address = GetProcAddress(FilesystemDll, "?add_source@fs@@YA_NPEBDW4ENUM@FFSAddSourceFlags@@@Z");
	CLogV_Address = GetProcAddress(FilesystemDll, "?_CLogV@@YAXW4TYPE@ELevel@Log@@PEBD1HW4ENUM@CLFilterAction@@W44CLLineMode@@1PEAD@Z");
	GetCategoryLevel_Address = GetProcAddress(FilesystemDll, "?GetCategoryLevel@Settings@Log@@QEBA?AW4TYPE@ELevel@2@PEBD@Z");

	if (MH_CreateHook(InitializeGameScript_Address, &InitializeGameScript_Hook, reinterpret_cast<void**>(&InitializeGameScript)) != MH_OK)
	{
		std::string errorMessage = std::string("Unable to create InitializeGameScript hook at location: %p"), InitializeGameScript_Address;
		Log(errorMessage);
		throw std::runtime_error(errorMessage);
	}
	if (MH_CreateHook(Add_Source_Address, &Add_Source_Hook, reinterpret_cast<void**>(&Add_Source)) != MH_OK)
	{
		std::string errorMessage = std::string("Unable to create Add_Source hook at location: %p"), InitializeGameScript_Address;
		Log(errorMessage);
		throw std::runtime_error(errorMessage);
	}
	if (MH_CreateHook(CLogV_Address, &CLogV_Hook, reinterpret_cast<void**>(&CLogV)) != MH_OK)
	{
		std::string errorMessage = std::string("Unable to create CLogV hook at location: %p"), InitializeGameScript_Address;
		Log(errorMessage);
		throw std::runtime_error(errorMessage);
	}
	if (MH_CreateHook(GetCategoryLevel_Address, &GetCategoryLevel_Hook, reinterpret_cast<void**>(&GetCategoryLevel)) != MH_OK)
	{
		std::string errorMessage = std::string("Unable to create GetCategoryLevel hook at location: %p"), InitializeGameScript_Address;
		Log(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	//CreateTestHook();

	IndexPaks();
	MH_EnableHook(MH_ALL_HOOKS);

	bool init_hook = false;
	do
	{
		if (Directx11_Hook() == Status::Success)
		{
			if (::GetModuleHandle("dxgi.dll") != NULL && ::GetModuleHandle("d3d11.dll") != NULL)
			{
				bind(8, (void**)&oPresent, hkPresent);
				bind(13, (void**)&oResizeBuffers, hkResizeBuffers); //gota get that Resizeable Buffalo
				init_hook = true;
			}
		}

	} while (!init_hook);

	//need to do this like, good
	while (true) {
		Sleep(100);
		if (GetAsyncKeyState(VK_DELETE))
			break;
	}

	return true;
}