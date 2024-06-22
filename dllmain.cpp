// dllmain.cpp : Defines the entry point for the DLL application.
#include "DLML.h"

#include "Proxy/Dsound.h"


auto init() -> void {
	(void)std::make_unique<DLML>();
};

auto APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) -> BOOL {

	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {


		if (_DEBUG) {
			(void)AllocConsole();

			(void)freopen("CONOUT$", "w", stderr);
			(void)freopen("CONOUT$", "w", stdout);
		}

		(void)Dsound::init();//handle

		HANDLE handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)init, hModule, 0, nullptr);
		(void)SetThreadPriority(handle, 2);

	}

	return TRUE;
}
