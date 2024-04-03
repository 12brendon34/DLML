// dllmain.cpp : Defines the entry point for the DLL application.
#include "common.h"
#include "Hooks.h"
#include "global.h"

HMODULE dsound_dll;

/* This Should be a lot better. It will also allow debugging and it doesn't use asm */
typedef HRESULT(WINAPI* directsoundcapturecreate)(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUNDCAPTURE* ppDSC, _Pre_null_ LPUNKNOWN pUnkOuter);
directsoundcapturecreate DirectSoundCaptureCreate_Real;
HRESULT WINAPI DirectSoundCaptureCreate(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUNDCAPTURE* ppDSC, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return (DirectSoundCaptureCreate_Real)(pcGuidDevice, ppDSC, pUnkOuter);
}

typedef HRESULT(WINAPI* directsoundcapturecreate8)(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUNDCAPTURE8* ppDSC8, _Pre_null_ LPUNKNOWN pUnkOuter);
directsoundcapturecreate8 DirectSoundCaptureCreate8_Real;
HRESULT WINAPI DirectSoundCaptureCreate8(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUNDCAPTURE8* ppDSC8, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return (DirectSoundCaptureCreate8_Real)(pcGuidDevice, ppDSC8, pUnkOuter);
}

typedef HRESULT(WINAPI* directsoundcaptureenumeratea)(_In_ LPDSENUMCALLBACKA pDSEnumCallback, _In_opt_ LPVOID pContext);
directsoundcaptureenumeratea DirectSoundCaptureEnumerateA_Real;
HRESULT WINAPI DirectSoundCaptureEnumerateA(_In_ LPDSENUMCALLBACKA pDSEnumCallback, _In_opt_ LPVOID pContext)
{
	return (DirectSoundCaptureEnumerateA_Real)(pDSEnumCallback, pContext);
}

typedef HRESULT(WINAPI* directsoundcaptureenumeratew)(_In_ LPDSENUMCALLBACKW pDSEnumCallback, _In_opt_ LPVOID pContext);
directsoundcaptureenumeratew DirectSoundCaptureEnumerateW_Real;
HRESULT WINAPI DirectSoundCaptureEnumerateW(_In_ LPDSENUMCALLBACKW pDSEnumCallback, _In_opt_ LPVOID pContext)
{
	return (DirectSoundCaptureEnumerateW_Real)(pDSEnumCallback, pContext);
}

typedef _Check_return_ HRESULT(WINAPI* directsoundcreate)(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND* ppDS, _Pre_null_ LPUNKNOWN pUnkOuter);
directsoundcreate DirectSoundCreate_Real;
_Check_return_ HRESULT WINAPI DirectSoundCreate(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND* ppDS, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return (DirectSoundCreate_Real)(pcGuidDevice, ppDS, pUnkOuter);
}

typedef HRESULT(WINAPI* directsoundcreate8)(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND8* ppDS8, _Pre_null_ LPUNKNOWN pUnkOuter);
directsoundcreate8 DirectSoundCreate8_Real;
HRESULT WINAPI DirectSoundCreate8(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND8* ppDS8, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return (DirectSoundCreate8_Real)(pcGuidDevice, ppDS8, pUnkOuter);
}

typedef HRESULT(WINAPI* directsoundenumeratea)(_In_ LPDSENUMCALLBACKA pDSEnumCallback, _In_opt_ LPVOID pContext);
directsoundenumeratea DirectSoundEnumerateA_Real;
HRESULT WINAPI DirectSoundEnumerateA(_In_ LPDSENUMCALLBACKA pDSEnumCallback, _In_opt_ LPVOID pContext)
{
	return (DirectSoundEnumerateA_Real)(pDSEnumCallback, pContext);
}

typedef HRESULT(WINAPI* directsoundenumeratew)(_In_ LPDSENUMCALLBACKW pDSEnumCallback, _In_opt_ LPVOID pContext);
directsoundenumeratew DirectSoundEnumerateW_Real;
HRESULT WINAPI DirectSoundEnumerateW(_In_ LPDSENUMCALLBACKW pDSEnumCallback, _In_opt_ LPVOID pContext)
{
	return (DirectSoundEnumerateW_Real)(pDSEnumCallback, pContext);
}

typedef HRESULT(WINAPI* directsoundfullduplexcreate)(_In_opt_ LPCGUID pcGuidCaptureDevice, _In_opt_ LPCGUID pcGuidRenderDevice, _In_ LPCDSCBUFFERDESC pcDSCBufferDesc, _In_ LPCDSBUFFERDESC pcDSBufferDesc, HWND hWnd, DWORD dwLevel, _Outptr_ LPDIRECTSOUNDFULLDUPLEX* ppDSFD, _Outptr_ LPDIRECTSOUNDCAPTUREBUFFER8* ppDSCBuffer8, _Outptr_ LPDIRECTSOUNDBUFFER8* ppDSBuffer8, _Pre_null_ LPUNKNOWN pUnkOuter);
directsoundfullduplexcreate DirectSoundFullDuplexCreate_Real;
HRESULT WINAPI DirectSoundFullDuplexCreate(_In_opt_ LPCGUID pcGuidCaptureDevice,_In_opt_ LPCGUID pcGuidRenderDevice,_In_ LPCDSCBUFFERDESC pcDSCBufferDesc,_In_ LPCDSBUFFERDESC pcDSBufferDesc,HWND hWnd,DWORD dwLevel,_Outptr_ LPDIRECTSOUNDFULLDUPLEX* ppDSFD,_Outptr_ LPDIRECTSOUNDCAPTUREBUFFER8* ppDSCBuffer8,_Outptr_ LPDIRECTSOUNDBUFFER8* ppDSBuffer8,_Pre_null_ LPUNKNOWN pUnkOuter)
{
	return (DirectSoundFullDuplexCreate_Real)(pcGuidCaptureDevice, pcGuidRenderDevice, pcDSCBufferDesc, pcDSBufferDesc, hWnd, dwLevel, ppDSFD, ppDSCBuffer8, ppDSBuffer8, pUnkOuter);
}

typedef __control_entrypoint(DllExport) HRESULT(STDAPICALLTYPE* dllcanunloadnow)(void);
dllcanunloadnow DllCanUnloadNow_Real;
__control_entrypoint(DllExport) STDAPI DllCanUnloadNow(void)
{
	return (DllCanUnloadNow_Real)();
}

typedef _Check_return_ HRESULT(STDAPICALLTYPE* dllgetclassobject)(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv);
dllgetclassobject DllGetClassObject_Real;
_Check_return_ STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{
	return (DllGetClassObject_Real)(rclsid, riid, ppv);
}

typedef HRESULT(WINAPI* getdeviceid)(_In_opt_ LPCGUID pGuidSrc, _Out_ LPGUID pGuidDest);
getdeviceid GetDeviceID_Real;
HRESULT WINAPI GetDeviceID(_In_opt_ LPCGUID pGuidSrc, _Out_ LPGUID pGuidDest)
{
	return (GetDeviceID_Real)(pGuidSrc, pGuidDest);
}

void setup_dsound() {
	DirectSoundCaptureCreate_Real = (directsoundcapturecreate)GetProcAddress(dsound_dll, "DirectSoundCaptureCreate");
	DirectSoundCaptureCreate8_Real = (directsoundcapturecreate8)GetProcAddress(dsound_dll, "DirectSoundCaptureCreate8");
	DirectSoundCaptureEnumerateA_Real = (directsoundcaptureenumeratea)GetProcAddress(dsound_dll, "DirectSoundCaptureEnumerateA");
	DirectSoundCaptureEnumerateW_Real = (directsoundcaptureenumeratew)GetProcAddress(dsound_dll, "DirectSoundCaptureEnumerateW");
	DirectSoundCreate_Real = (directsoundcreate)GetProcAddress(dsound_dll, "DirectSoundCreate");
	DirectSoundCreate8_Real = (directsoundcreate8)GetProcAddress(dsound_dll, "DirectSoundCreate8");
	DirectSoundEnumerateA_Real = (directsoundenumeratea)GetProcAddress(dsound_dll, "DirectSoundEnumerateA");
	DirectSoundEnumerateW_Real = (directsoundenumeratew)GetProcAddress(dsound_dll, "DirectSoundEnumerateW");
	DirectSoundFullDuplexCreate_Real = (directsoundfullduplexcreate)GetProcAddress(dsound_dll, "DirectSoundFullDuplexCreate");
	DllCanUnloadNow_Real = (dllcanunloadnow)GetProcAddress(dsound_dll, "DllCanUnloadNow");
	DllGetClassObject_Real = (dllgetclassobject)GetProcAddress(dsound_dll, "DllGetClassObject");
	GetDeviceID_Real = (getdeviceid)GetProcAddress(dsound_dll, "GetDeviceID");
}

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

		char path[MAX_PATH]{};
		CopyMemory(path + GetSystemDirectory(path, MAX_PATH - 12), "\\dsound.dll", 12);
		dsound_dll = LoadLibrary(path);

		if (dsound_dll == NULL)
			MsgBoxExit(MB_ICONERROR, "Exiting", "Source dsound.dll missing");

		setup_dsound();

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
		(void)FreeLibrary(dsound_dll);
		(void)MH_Uninitialize();

		if (_DEBUG) {
			(void)FreeConsole();
		}
	}
	break;
	}
	return TRUE;
}
