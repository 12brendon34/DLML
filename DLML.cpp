#include "DLML.h"

DLML::DLML() {

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


	Loader::IndexPaks();
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

	HookRenderer();
}

//bool is_initialized = false;

kiero::RenderType::Enum rendererAPI = kiero::RenderType::D3D11;

auto DLML::HookRenderer(void) -> void {
	/*
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

		if (kiero::init(rendererAPI) != kiero::status::Success)
			continue;


		if (rendererAPI == kiero::RenderType::D3D11)
			impl::d3d11::init();
		else
			impl::d3d12::init();


		init_hook = true;

	} while (!init_hook);
	*/
	bool init_hook = false;
	do {
		if (kiero::init(rendererAPI) != kiero::status::Success)
			continue;

		impl::d3d11::init();

	} while (!init_hook);
}

auto DLML::initMinHook(void) const -> Status {

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

	new Add_Source(this);
	new InitalizeGameScript(this);

	for (auto hook : this->hooks) {

		auto [hookStatus, statusCode] = hook->init();
		//if hookStatus err
		//log data.second

	};

	(void)MH_EnableHook(MH_ALL_HOOKS);

	return Status(MethodStatus::Success, "Hooks Initalized");
}
