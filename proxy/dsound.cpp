#include "dsound.h"

HMODULE dsound_dll;
using t_DirectSoundCaptureCreate = HRESULT (WINAPI*)(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUNDCAPTURE* ppDSC, _Pre_null_ LPUNKNOWN pUnkOuter);
using t_DirectSoundCaptureCreate8 = HRESULT(WINAPI*)(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUNDCAPTURE8* ppDSC8, _Pre_null_ LPUNKNOWN pUnkOuter);
using t_DirectSoundCaptureEnumerateA = HRESULT(WINAPI*)(_In_ LPDSENUMCALLBACKA pDSEnumCallback, _In_opt_ LPVOID pContext);
using t_DirectSoundCaptureEnumerateW = HRESULT(WINAPI*)(_In_ LPDSENUMCALLBACKW pDSEnumCallback, _In_opt_ LPVOID pContext);
using t_DirectSoundCreate = HRESULT(WINAPI*)(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND* ppDS, _Pre_null_ LPUNKNOWN pUnkOuter);
using t_DirectSoundCreate8 = HRESULT(WINAPI*)(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND8* ppDS8, _Pre_null_ LPUNKNOWN pUnkOuter);
using t_DirectSoundEnumerateA = HRESULT(WINAPI*)(_In_ LPDSENUMCALLBACKA pDSEnumCallback, _In_opt_ LPVOID pContext);
using t_DirectSoundEnumerateW = HRESULT(WINAPI*)(_In_ LPDSENUMCALLBACKW pDSEnumCallback, _In_opt_ LPVOID pContext);
using t_DirectSoundFullDuplexCreate = HRESULT(WINAPI*)(_In_opt_ LPCGUID pcGuidCaptureDevice, _In_opt_ LPCGUID pcGuidRenderDevice, _In_ LPCDSCBUFFERDESC pcDSCBufferDesc, _In_ LPCDSBUFFERDESC pcDSBufferDesc, HWND hWnd, DWORD dwLevel, _Outptr_ LPDIRECTSOUNDFULLDUPLEX* ppDSFD, _Outptr_ LPDIRECTSOUNDCAPTUREBUFFER8* ppDSCBuffer8, _Outptr_ LPDIRECTSOUNDBUFFER8* ppDSBuffer8, _Pre_null_ LPUNKNOWN pUnkOuter);
using t_GetDeviceID = HRESULT(WINAPI*)(_In_opt_ LPCGUID pGuidSrc, _Out_ LPGUID pGuidDest);

t_DirectSoundCaptureCreate o_DirectSoundCaptureCreate;
t_DirectSoundCaptureCreate8 o_DirectSoundCaptureCreate8;
t_DirectSoundCaptureEnumerateA o_DirectSoundCaptureEnumerateA;
t_DirectSoundCaptureEnumerateW o_DirectSoundCaptureEnumerateW;
t_DirectSoundCreate o_DirectSoundCreate;
t_DirectSoundCreate8 o_DirectSoundCreate8;
t_DirectSoundEnumerateA o_DirectSoundEnumerateA;
t_DirectSoundEnumerateW o_DirectSoundEnumerateW;
t_DirectSoundFullDuplexCreate o_DirectSoundFullDuplexCreate;
t_GetDeviceID o_GetDeviceID;

HRESULT WINAPI hkDirectSoundCaptureCreate(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUNDCAPTURE* ppDSC, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return o_DirectSoundCaptureCreate(pcGuidDevice, ppDSC, pUnkOuter);
}

HRESULT WINAPI hkDirectSoundCaptureCreate8(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUNDCAPTURE8* ppDSC8, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return o_DirectSoundCaptureCreate8(pcGuidDevice, ppDSC8, pUnkOuter);
}

HRESULT WINAPI hkDirectSoundCaptureEnumerateA(_In_ LPDSENUMCALLBACKA pDSEnumCallback, _In_opt_ LPVOID pContext)
{
	return o_DirectSoundCaptureEnumerateA(pDSEnumCallback, pContext);
}

HRESULT WINAPI hkDirectSoundCaptureEnumerateW(_In_ LPDSENUMCALLBACKW pDSEnumCallback, _In_opt_ LPVOID pContext)
{
	return o_DirectSoundCaptureEnumerateW(pDSEnumCallback, pContext);
}

_Check_return_ HRESULT WINAPI hkDirectSoundCreate(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND* ppDS, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return o_DirectSoundCreate(pcGuidDevice, ppDS, pUnkOuter);
}

HRESULT WINAPI hkDirectSoundCreate8(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND8* ppDS8, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return o_DirectSoundCreate8(pcGuidDevice, ppDS8, pUnkOuter);
}

HRESULT WINAPI hkDirectSoundEnumerateA(_In_ LPDSENUMCALLBACKA pDSEnumCallback, _In_opt_ LPVOID pContext)
{
	return o_DirectSoundEnumerateA(pDSEnumCallback, pContext);
}

HRESULT WINAPI hkDirectSoundEnumerateW(_In_ LPDSENUMCALLBACKW pDSEnumCallback, _In_opt_ LPVOID pContext)
{
	return o_DirectSoundEnumerateW(pDSEnumCallback, pContext);
}

HRESULT WINAPI hkDirectSoundFullDuplexCreate(_In_opt_ LPCGUID pcGuidCaptureDevice, _In_opt_ LPCGUID pcGuidRenderDevice, _In_ LPCDSCBUFFERDESC pcDSCBufferDesc, _In_ LPCDSBUFFERDESC pcDSBufferDesc, HWND hWnd, DWORD dwLevel, _Outptr_ LPDIRECTSOUNDFULLDUPLEX* ppDSFD, _Outptr_ LPDIRECTSOUNDCAPTUREBUFFER8* ppDSCBuffer8, _Outptr_ LPDIRECTSOUNDBUFFER8* ppDSBuffer8, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return o_DirectSoundFullDuplexCreate(pcGuidCaptureDevice, pcGuidRenderDevice, pcDSCBufferDesc, pcDSBufferDesc, hWnd, dwLevel, ppDSFD, ppDSCBuffer8, ppDSBuffer8, pUnkOuter);
}

HRESULT WINAPI hkGetDeviceID(_In_opt_ LPCGUID pGuidSrc, _Out_ LPGUID pGuidDest)
{
	return o_GetDeviceID(pGuidSrc, pGuidDest);
}

auto Dsound::init(void) -> HMODULE {

	char path[MAX_PATH]{};
	CopyMemory(path + GetSystemDirectory(path, MAX_PATH - 11), "\\dsound.dll", 11);
	dsound_dll = LoadLibrary(path);

	if (dsound_dll == NULL)
		Utils::MsgBoxExit(MB_ICONERROR, "Exiting", "Source dsound.dll missing");

	o_DirectSoundCaptureCreate = std::bit_cast<t_DirectSoundCaptureCreate>(GetProcAddress(dsound_dll, "DirectSoundCaptureCreate"));
	o_DirectSoundCaptureCreate8 = std::bit_cast<t_DirectSoundCaptureCreate8>(GetProcAddress(dsound_dll, "DirectSoundCaptureCreate8"));
	o_DirectSoundCaptureEnumerateA = std::bit_cast<t_DirectSoundCaptureEnumerateA>(GetProcAddress(dsound_dll, "DirectSoundCaptureEnumerateA"));
	o_DirectSoundCaptureEnumerateW = std::bit_cast<t_DirectSoundCaptureEnumerateW>(GetProcAddress(dsound_dll, "DirectSoundCaptureEnumerateW"));
	o_DirectSoundCreate = std::bit_cast<t_DirectSoundCreate>(GetProcAddress(dsound_dll, "DirectSoundCreate"));
	o_DirectSoundCreate8 = std::bit_cast<t_DirectSoundCreate8>(GetProcAddress(dsound_dll, "DirectSoundCreate8"));
	o_DirectSoundEnumerateA = std::bit_cast<t_DirectSoundEnumerateA>(GetProcAddress(dsound_dll, "DirectSoundEnumerateA"));
	o_DirectSoundEnumerateW = std::bit_cast<t_DirectSoundEnumerateW>(GetProcAddress(dsound_dll, "DirectSoundEnumerateW"));
	o_DirectSoundFullDuplexCreate = std::bit_cast<t_DirectSoundFullDuplexCreate>(GetProcAddress(dsound_dll, "DirectSoundFullDuplexCreate"));
	o_GetDeviceID = std::bit_cast<t_GetDeviceID>(GetProcAddress(dsound_dll, "GetDeviceID"));


	return dsound_dll;
}