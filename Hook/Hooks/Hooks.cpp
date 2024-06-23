#include "Hooks.h"
#include "../../Loader.h"

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