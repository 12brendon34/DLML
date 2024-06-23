#include "Hooks.h"

using tAdd_Source = bool (*)(const char*, int);
tAdd_Source oAdd_Source;

auto HkAdd_Source(char const* Path, int FFSAddSourceFlags) -> bool {
	(void)dbgprintf("FS::Add_Source Path: %s, Flags: %i\n", Path, FFSAddSourceFlags);
	return oAdd_Source(Path, FFSAddSourceFlags);
};


auto Add_Source::init(void) -> Status {

	FARPROC Add_Source_Address = Utils::GetProcAddressSimple(this->pDLML->FilesystemDll, "?add_source@fs@@YA_NPEBDW4ENUM@FFSAddSourceFlags@@@Z");
	(void)Utils::HookFunction(Add_Source_Address, &HkAdd_Source, std::bit_cast<void**>(&oAdd_Source), "fs::add_source");

	return Status(MethodStatus::Success, "[Add_Source] Successfully hooked");
};


using tInitalizeGameScript = void  (*)(LPCSTR locale);
tInitalizeGameScript oInitalizeGameScript;

auto HkInitalizeGameScript(LPCSTR locale) -> void {
	for (size_t i = 0; i < Loader::ModInfoList.size(); i++)
		if (Loader::ModInfoList[i].ModType == 0)
			(void)HkAdd_Source(Loader::ModInfoList[i].ModPath.c_str(), 9);

	return oInitalizeGameScript(locale);
};

auto InitalizeGameScript::init(void) -> Status {
	
	FARPROC InitalizeGameScript_Address = Utils::GetProcAddressSimple(this->pDLML->EngineDll, "InitializeGameScript");
	(void)Utils::HookFunction(InitalizeGameScript_Address, &HkInitalizeGameScript, std::bit_cast<void**>(&oInitalizeGameScript), "CGame::InitalizeGameScript");

	return Status(MethodStatus::Success, "[InitalizeGameScript] Successfully hooked");
};

using tCLogV = void  (*)(int type, char const* param_2, char const* param_3, int param_4, int CLFilterAction, int CLLineMode, char const* param_7, va_list param_8);
tCLogV oCLogV;

enum LogType {
	DEBG = 5,
	WARN = 4,//prob warning?? not actually 100% sure
	INFO = 3,
	ERRR = 2
};
//					enum Log::ELevel::TYPE											enum CLFilterAction::ENUM enum CLLineMode::ENUM
auto __cdecl hkCLogV(LogType type, char const* thread, char const* sourcefile, int linenumber, int CLFilterAction, int CLLineMode, char const* message, va_list printarg) -> void {
	oCLogV(type, thread, sourcefile, linenumber, CLFilterAction, CLLineMode, message, printarg);

	std::string Message;

	switch (type)
	{
		case DEBG:
			Message = " DBUG :";
		case WARN:
			Message = " WARN :";
		case INFO:
			Message = " INFO :";
		case ERRR:
			Message = " ERRR :";
		default:
			Message = " INFO :";
	}

	Message.append(Utils::srprintf( " [%s] ", thread));

	if (Menu::sourceFiles)
		Message.append(Utils::srprintf( " >%s(%i) ", sourcefile, linenumber));

	if (CLFilterAction == 2)
		Message.append(Utils::srprintf( "| "));
	else
		Message.append(Utils::srprintf( "> "));

	char Buffer[65528];
	vsprintf(Buffer, message, printarg);
	Message.append(Buffer);

	Menu::AddLog(Message.c_str());
};

auto CLogV::init(void) -> Status {

	FARPROC CLogV_Address = Utils::GetProcAddressSimple(this->pDLML->FilesystemDll, "?_CLogV@@YAXW4TYPE@ELevel@Log@@PEBD1HW4ENUM@CLFilterAction@@W44CLLineMode@@1PEAD@Z");
	(void)Utils::HookFunction(CLogV_Address, &hkCLogV, std::bit_cast<void**>(&oCLogV), "_CLogV");

	return Status(MethodStatus::Success, "[CLogV] Successfully hooked");
};

int hkGetCategoryLevel(int This, char* Catagory) { //GetCategoryLevel
	return INT_MAX;
}

auto GetCategoryLevel::init(void) -> Status {

	FARPROC GetCategoryLevel_Address = Utils::GetProcAddressSimple(this->pDLML->FilesystemDll, "?GetCategoryLevel@Settings@Log@@QEBA?AW4TYPE@ELevel@2@PEBD@Z");
	(void)Utils::HookFunction(GetCategoryLevel_Address, &hkGetCategoryLevel, nullptr, "Log::Settings::GetCategoryLevel");

	return Status(MethodStatus::Success, "[Log::Settings::GetCategoryLevel] Successfully hooked");
}

using tLogSettingsInstance = int* (*)();
tLogSettingsInstance oLogSettingsInstance;
int* hkLog_Settings_Instance() { //GetCategoryLevel
	int* logint = oLogSettingsInstance();
	*logint = INT_MAX;

	return logint;
}

auto Log_Settings_Instance::init(void) -> Status {

	FARPROC Log_Settings_Instance_Address = Utils::GetProcAddressSimple(this->pDLML->FilesystemDll, "?Instance@Settings@Log@@SAAEAV12@XZ");
	(void)Utils::HookFunction(Log_Settings_Instance_Address, &hkLog_Settings_Instance, std::bit_cast<void**>(&oLogSettingsInstance), "Log::Settings::Instance");

	return Status(MethodStatus::Success, "[Log::Settings::Instance] Successfully hooked");
}