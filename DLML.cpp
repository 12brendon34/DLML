#include "DLML.h"
#include "Hook/Hooks/Add_Source/Add_Source.h"

DLML::DLML() {
	while (!::IsDebuggerPresent())
		::Sleep(100); // to avoid 100% CPU load

	auto [minhookStatus, minhookStatusCode] = this->initMinHook();


	switch (minhookStatus) {

		case MethodStatus::Success: {
			dbgprintf("%s\n", minhookStatusCode.c_str());
			break;
		}
		case MethodStatus::Error: {
			Utils::MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to initialize MinHook: %p", minhookStatusCode.c_str());
			break;
		}
	}



	auto [hookStatus, hookStatusCode] = this->initHooks();

	switch (hookStatus) {

		case MethodStatus::Success:{
			dbgprintf("%s\n", hookStatusCode.c_str());
			break;
		}
		case MethodStatus::Error:{
			Utils::MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to initialize Hook: %p", hookStatusCode.c_str());
			break;
		}
	}

	dbgprintf("DLML Initialized\n");
}

auto DLML::initMinHook(void) -> Status {

	MH_STATUS status = MH_Initialize();
	std::string statusCode = MH_StatusToString(status);

	if (status == MH_OK || status == MH_ERROR_ALREADY_INITIALIZED) {
		return Status(MethodStatus::Success, "Initalized Minhook");
	}

	return Status(MethodStatus::Error, statusCode);
}

auto DLML::initHooks(void) -> Status {

	DLML::FilesystemDll = Utils::GetModuleHandleSimple("filesystem_x64_rwdi.dll");
	DLML::EngineDll = Utils::GetModuleHandleSimple("engine_x64_rwdi.dll");

	(void)std::make_unique<Add_Source>(this);

	for (auto hook : this->hooks) {

		auto [hookStatus, statusCode] = hook->init();
		//if hookStatus err
		//log data.second

	};

	(void)MH_EnableHook(MH_ALL_HOOKS);

	return Status(MethodStatus::Success, "Hooks Initalized");
}
