// dllmain.cpp : Defines the entry point for the DLL application.
#include "common.h"
#include "Hooks.h"
#include "global.h"
#include "proxy/winmm.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		(void)DisableThreadLibraryCalls(hModule);

		if (_DEBUG) {
			(void)AllocConsole();

			(void)freopen("CONOUT$", "w", stderr);
			(void)freopen("CONOUT$", "w", stdout);
		}

		MH_STATUS status = MH_Initialize();
		std::string statusCode = MH_StatusToString(status);

		if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED) {
			MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to initialize MinHook: %p", statusCode);
		}
		globals.MinHookInitialized = true;

		winmm();

		HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CreateHooks, hModule, 0, 0);
		if (handle != 0)
		{
			(void)SetThreadPriority(handle, 2);
			(void)CloseHandle(handle);
		}

		break;
	}
	case DLL_PROCESS_DETACH:
	{
		(void)winmm(true);
		(void)MH_Uninitialize();

		if (_DEBUG) {
			(void)FreeConsole();
		}
	}
	break;
	}
	return TRUE;
}
