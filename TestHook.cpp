#include "MinHook/MinHook.h"
#include <string>
#include <stdexcept>
#include "Logger.h"

typedef int(*function)(int This, char* param_1);
function Function;

int Function_Hook(int This, char* param_1) { //GetCategoryLevel
	//Log(param_1);
	//bool Status = Function(NULL, param_1);

	return 100;
}

void CreateTestHook() {

	Log("Hooking TestHook for GetCatagoryLevel");
	HMODULE FilesystemDll = GetModuleHandleA("filesystem_x64_rwdi.dll");

	FARPROC Function_Address = GetProcAddress(FilesystemDll, "?GetCategoryLevel@Settings@Log@@QEBA?AW4TYPE@ELevel@2@PEBD@Z");

	if (MH_CreateHook(Function_Address, &Function_Hook, reinterpret_cast<void**>(&Function)) != MH_OK)
	{
		std::string errorMessage = std::string("Unable to create Test Hook at location: %p"), InitializeGameScript_Address;
		Log(errorMessage);
		throw std::runtime_error(errorMessage);
	}
}