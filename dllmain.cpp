// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include "dsound.h"

HMODULE dsound_dll;

/* This Should be a lot better. It will also allow debugging and it doesn't use asm */
typedef HRESULT(WINAPI* directsoundcapturecreate)(LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN Unknown);
directsoundcapturecreate DirectSoundCaptureCreate_Real;
HRESULT WINAPI DirectSoundCaptureCreate(LPCGUID pcGuidDevice, LPDIRECTSOUND * ppDS, LPUNKNOWN Unknown)
{
	return (DirectSoundCaptureCreate_Real)(pcGuidDevice, ppDS, Unknown);
}

typedef HRESULT(WINAPI* directsoundcapturecreate8)(LPCGUID pcGuidDevice, LPDIRECTSOUNDCAPTURE8 * ppDSC8, LPUNKNOWN Unknown);
directsoundcapturecreate8 DirectSoundCaptureCreate8_Real;
HRESULT WINAPI DirectSoundCaptureCreate8(LPCGUID pcGuidDevice, LPDIRECTSOUNDCAPTURE8* ppDSC8, LPUNKNOWN Unknown)
{
	return (DirectSoundCaptureCreate8_Real)(pcGuidDevice, ppDSC8, Unknown);
}

typedef HRESULT(WINAPI* directsoundcaptureenumeratea)(LPDSENUMCALLBACKA pDSEnumCallback, LPVOID pContext);
directsoundcaptureenumeratea DirectSoundCaptureEnumerateA_Real;
HRESULT WINAPI DirectSoundCaptureEnumerateA(LPDSENUMCALLBACKA pDSEnumCallback, LPVOID pContext)
{
	return (DirectSoundCaptureEnumerateA_Real)(pDSEnumCallback, pContext);
}

typedef HRESULT(WINAPI* directsoundcaptureenumeratew)(LPDSENUMCALLBACKW pDSEnumCallback, LPVOID pContext);
directsoundcaptureenumeratew DirectSoundCaptureEnumerateW_Real;
HRESULT WINAPI DirectSoundCaptureEnumerateW(LPDSENUMCALLBACKW pDSEnumCallback, LPVOID pContext)
{
	return (DirectSoundCaptureEnumerateW_Real)(pDSEnumCallback, pContext);
}

typedef HRESULT(WINAPI* directsoundcreate)(LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN Unknown);
directsoundcreate DirectSoundCreate_Real;
HRESULT WINAPI DirectSoundCreate(LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN Unknown)
{
	return (DirectSoundCreate_Real)(pcGuidDevice, ppDS, Unknown);
}

typedef HRESULT(WINAPI* directsoundcreate8)(LPCGUID pcGuidDevice, LPDIRECTSOUND8 * ppDS8, LPUNKNOWN Unknown);
directsoundcreate8 DirectSoundCreate8_Real;
HRESULT WINAPI DirectSoundCreate8(LPCGUID pcGuidDevice, LPDIRECTSOUND8* ppDS8, LPUNKNOWN Unknown)
{
	return (DirectSoundCreate8_Real)(pcGuidDevice, ppDS8, Unknown);
}

typedef HRESULT(WINAPI* directsoundenumeratea)(LPDSENUMCALLBACKA pDSEnumCallback, LPVOID pContext);
directsoundenumeratea DirectSoundEnumerateA_Real;
HRESULT WINAPI DirectSoundEnumerateA(LPDSENUMCALLBACKA pDSEnumCallback, LPVOID pContext)
{
	return (DirectSoundEnumerateA_Real)(pDSEnumCallback, pContext);
}

typedef HRESULT(WINAPI* directsoundenumeratew)(LPDSENUMCALLBACKW pDSEnumCallback, LPVOID pContext);
directsoundenumeratew DirectSoundEnumerateW_Real;
HRESULT WINAPI DirectSoundEnumerateW(LPDSENUMCALLBACKW pDSEnumCallback, LPVOID pContext)
{
	return (DirectSoundEnumerateW_Real)(pDSEnumCallback, pContext);
}

typedef HRESULT(WINAPI* directsoundfullduplexcreate)(LPCGUID pcGuidCaptureDevice, LPCGUID pcGuidRenderDevice, LPCDSCBUFFERDESC pcDSCBufferDesc, LPCDSBUFFERDESC pcDSBufferDesc, HWND hWnd, DWORD dwLevel, LPDIRECTSOUNDFULLDUPLEX* ppDSFD, LPDIRECTSOUNDCAPTUREBUFFER8* ppDSCBuffer8, LPDIRECTSOUNDBUFFER8* ppDSBuffer8, LPUNKNOWN Unknown);
directsoundfullduplexcreate DirectSoundFullDuplexCreate_Real;
HRESULT WINAPI DirectSoundFullDuplexCreate(LPCGUID pcGuidCaptureDevice, LPCGUID pcGuidRenderDevice, LPCDSCBUFFERDESC pcDSCBufferDesc, LPCDSBUFFERDESC pcDSBufferDesc, HWND hWnd, DWORD dwLevel, LPDIRECTSOUNDFULLDUPLEX* ppDSFD, LPDIRECTSOUNDCAPTUREBUFFER8* ppDSCBuffer8, LPDIRECTSOUNDBUFFER8* ppDSBuffer8, LPUNKNOWN Unknown)
{
	return (DirectSoundFullDuplexCreate_Real)(pcGuidCaptureDevice, pcGuidRenderDevice, pcDSCBufferDesc, pcDSBufferDesc, hWnd, dwLevel, ppDSFD, ppDSCBuffer8, ppDSBuffer8, Unknown);
}

typedef HRESULT(WINAPI* dllcanunloadnow)();
dllcanunloadnow DllCanUnloadNow_Real;
HRESULT WINAPI DllCanUnloadNow()
{
	return (DllCanUnloadNow_Real)();
}

typedef HRESULT(WINAPI* dllgetclassobject)(IN REFCLSID rclsid, IN REFIID riid, OUT LPVOID FAR* ppv);
dllgetclassobject DllGetClassObject_Real;
HRESULT WINAPI DllGetClassObject(IN REFCLSID rclsid, IN REFIID riid, OUT LPVOID FAR * ppv)
{
	return (DllGetClassObject_Real)(rclsid, riid, ppv);
}

typedef HRESULT(WINAPI* getdeviceid)(LPCGUID pGuidSrc, LPGUID pGuidDest);
getdeviceid GetDeviceID_Real;
HRESULT WINAPI GetDeviceID(LPCGUID pGuidSrc, LPGUID pGuidDest)
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
	char path[MAX_PATH];
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		CopyMemory(path + GetSystemDirectory(path, MAX_PATH - 12), "\\dsound.dll", 12);
		dsound_dll = LoadLibrary(path);

		if (dsound_dll == NULL)
		{
			MessageBox(0, "Original dsound.dll missing", "Exiting", MB_ICONERROR);
			ExitProcess(0);
		}
		setup_dsound();

		break;
	}
	case DLL_PROCESS_DETACH:
	{
		FreeLibrary(dsound_dll);
	}
	break;
	}
	return TRUE;
}
