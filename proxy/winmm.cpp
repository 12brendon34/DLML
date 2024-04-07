#include "winmm.h"
#include "../common.h"

HMODULE winmm_dll;

typedef BOOL(WINAPI* mciexecute)(LPCSTR Param1);
mciexecute mciExecute_Real;
BOOL WINAPI mciExecute(LPCSTR Param1) {

	return (mciExecute_Real)(Param1);
}

typedef LRESULT(WINAPI* closedriver)(HDRVR hDriver, LPARAM lParam1, LPARAM lParam2);
closedriver CloseDriver_Real;
LRESULT WINAPI CloseDriver(HDRVR hDriver, LPARAM lParam1, LPARAM lParam2) {

	return (CloseDriver_Real)(hDriver, lParam1, lParam2);
}

typedef LRESULT(WINAPI* defdriverproc)(DWORD_PTR dwDriverIdentifier, HANDLE hdrvr, UINT uMsg, LPARAM lParam1, LPARAM lParam2);
defdriverproc DefDriverProc_Real;
LRESULT WINAPI DefDriverProc(DWORD_PTR dwDriverIdentifier, HANDLE hdrvr, UINT uMsg, LPARAM lParam1, LPARAM lParam2) {
	return (DefDriverProc_Real)(dwDriverIdentifier, hdrvr, uMsg, lParam1, lParam2);
}


typedef LRESULT(WINAPI* drivercallback)(DWORD_PTR dwCallback, DWORD dwFlags, HDRVR hDevice, DWORD dwMsg, DWORD_PTR dwUser, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
drivercallback DriverCallback_Real;
BOOL APIENTRY DriverCallback(DWORD_PTR dwCallback, DWORD dwFlags, HDRVR hDevice, DWORD dwMsg, DWORD_PTR dwUser, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	return (DriverCallback_Real)(dwCallback, dwFlags, hDevice, dwMsg, dwUser, dwParam1, dwParam2);
}

typedef HMODULE(WINAPI* drvgetmodulehandle)(HDRVR hDriver);
drvgetmodulehandle DrvGetModuleHandle_Real;
HMODULE WINAPI DrvGetModuleHandle(HDRVR hDriver)
{
	return (DrvGetModuleHandle_Real)(hDriver);
}

typedef HMODULE(WINAPI* getdrivermodulehandle)(HDRVR hDriver);
getdrivermodulehandle GetDriverModuleHandle_Real;
HMODULE WINAPI GetDriverModuleHandle(HDRVR hDriver)
{
	return (GetDriverModuleHandle_Real)(hDriver);
}

typedef HDRVR(WINAPI* opendriver)(LPCWSTR szDriverName, LPCWSTR szSectionName, LPARAM lParam2);
opendriver OpenDriver_Real;
HDRVR WINAPI OpenDriver(LPCWSTR szDriverName, LPCWSTR szSectionName, LPARAM lParam2)
{
	return (OpenDriver_Real)(szDriverName, szSectionName, lParam2);
}

typedef BOOL(WINAPI* playsound)(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound);
playsound PlaySound_Real;

/*
* I need to redo my naming, can't keep doing _Real and hoping the functions don't already have a body
* maybe something like orig_Function and hkFunction
* maybe _t for the typedef's, 
*/
BOOL WINAPI _PlaySound(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
	return (PlaySound_Real)(pszSound, hmod, fdwSound);
}

typedef BOOL(WINAPI* playsounda)(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound);
playsounda PlaySoundA_Real;
BOOL WINAPI PlaySoundA(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
	return (PlaySoundA_Real)(pszSound, hmod, fdwSound);
}

typedef BOOL(WINAPI* playsoundw)(LPCWSTR pszSound, HMODULE hmod, DWORD fdwSound);
playsoundw PlaySoundW_Real;
BOOL WINAPI PlaySoundW(LPCWSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
	return (PlaySoundW_Real)(pszSound, hmod, fdwSound);
}

typedef LRESULT(WINAPI* senddrivermessage)(HDRVR hDriver, UINT message, LPARAM lParam1, LPARAM lParam2);
senddrivermessage SendDriverMessage_Real;
LRESULT WINAPI SendDriverMessage(HDRVR hDriver, UINT message, LPARAM lParam1, LPARAM lParam2)
{
	return (SendDriverMessage_Real)(hDriver, message, lParam1, lParam2);
}

typedef void(WINAPI* wowappexit)();
wowappexit WOWAppExit_Real;
void WINAPI WOWAppExit()
{
	return (WOWAppExit_Real)();
}

typedef MMRESULT(WINAPI* auxgetdevcapsa)(UINT_PTR uDeviceID, LPAUXCAPSA pac, UINT cbac);
auxgetdevcapsa auxGetDevCapsA_Real;
MMRESULT WINAPI auxGetDevCapsA(UINT_PTR uDeviceID, LPAUXCAPSA pac, UINT cbac)
{
	return (auxGetDevCapsA_Real)(uDeviceID, pac, cbac);
}

typedef MMRESULT(WINAPI* auxgetdevcapsw)(UINT_PTR uDeviceID, LPAUXCAPSW pac, UINT cbac);
auxgetdevcapsw auxGetDevCapsW_Real;
MMRESULT WINAPI auxGetDevCapsW(UINT_PTR uDeviceID, LPAUXCAPSW pac, UINT cbac)
{
	return (auxGetDevCapsW_Real)(uDeviceID, pac, cbac);
}

typedef UINT(WINAPI* auxgetnumdevs)();
auxgetnumdevs auxGetNumDevs_Real;
UINT WINAPI auxGetNumDevs()
{
	return (auxGetNumDevs_Real)();
}

typedef MMRESULT(WINAPI* auxgetvolume)(UINT uDeviceID, LPDWORD pdwVolume);
auxgetvolume auxGetVolume_Real;
MMRESULT WINAPI auxGetVolume(UINT uDeviceID, LPDWORD pdwVolume)
{
	return (auxGetVolume_Real)(uDeviceID, pdwVolume);
}

typedef MMRESULT(WINAPI* auxoutmessage)(UINT uDeviceID, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
auxoutmessage auxOutMessage_Real;
MMRESULT WINAPI auxOutMessage(UINT uDeviceID, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
	return (auxOutMessage_Real)(uDeviceID, uMsg, dw1, dw2);
}

typedef MMRESULT(WINAPI* auxsetvolume)(UINT uDeviceID, DWORD dwVolume);
auxsetvolume auxSetVolume_Real;
MMRESULT WINAPI auxSetVolume(UINT uDeviceID, DWORD dwVolume)
{
	return (auxSetVolume_Real)(uDeviceID, dwVolume);
}

typedef MMRESULT(WINAPI* joyconfigchanged)(DWORD dwFlags);
joyconfigchanged joyConfigChanged_Real;
MMRESULT WINAPI joyConfigChanged(DWORD dwFlags)
{
	return (joyConfigChanged_Real)(dwFlags);
}

typedef MMRESULT(WINAPI* joygetdevcapsa)(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc);
joygetdevcapsa joyGetDevCapsA_Real;
MMRESULT WINAPI joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc)
{
	return (joyGetDevCapsA_Real)(uJoyID, pjc, cbjc);
}

typedef MMRESULT(WINAPI* joygetdevcapsw)(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc);
joygetdevcapsw joyGetDevCapsW_Real;
MMRESULT WINAPI joyGetDevCapsW(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc)
{
	return (joyGetDevCapsW_Real)(uJoyID, pjc, cbjc);
}

typedef UINT(WINAPI* joygetnumdevs)();
joygetnumdevs joyGetNumDevs_Real;
UINT WINAPI joyGetNumDevs(void)
{
	return (joyGetNumDevs_Real)();
}

typedef MMRESULT(WINAPI* joygetpos)(UINT uJoyID, LPJOYINFO pji);
joygetpos joyGetPos_Real;
MMRESULT WINAPI joyGetPos(UINT uJoyID, LPJOYINFO pji)
{
	return (joyGetPos_Real)(uJoyID, pji);
}

typedef MMRESULT(WINAPI* joygetposex)(UINT uJoyID, LPJOYINFOEX pji);
joygetposex joyGetPosEx_Real;
MMRESULT WINAPI joyGetPosEx(UINT uJoyID, LPJOYINFOEX pji)
{
	return (joyGetPosEx_Real)(uJoyID, pji);
}

typedef MMRESULT(WINAPI* joygetthreshold)(UINT uJoyID, LPUINT puThreshold);
joygetthreshold joyGetThreshold_Real;
MMRESULT WINAPI joyGetThreshold(UINT uJoyID, LPUINT puThreshold)
{
	return (joyGetThreshold_Real)(uJoyID, puThreshold);
}

typedef MMRESULT(WINAPI* joyreleasecapture)(UINT uJoyID);
joyreleasecapture joyReleaseCapture_Real;
MMRESULT WINAPI joyReleaseCapture(UINT uJoyID)
{
	return (joyReleaseCapture_Real)(uJoyID);
}

typedef MMRESULT(WINAPI* joysetcapture)(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged);
joysetcapture joySetCapture_Real;
MMRESULT WINAPI joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged)
{
	return (joySetCapture_Real)(hwnd, uJoyID, uPeriod, fChanged);
}

typedef MMRESULT(WINAPI* joysetthreshold)(UINT uJoyID, UINT uThreshold);
joysetthreshold joySetThreshold_Real;
MMRESULT WINAPI joySetThreshold(UINT uJoyID, UINT uThreshold)
{
	return (joySetThreshold_Real)(uJoyID, uThreshold);
}

typedef BOOL(WINAPI* mcidrivernotify)(HANDLE hwndCallback, MCIDEVICEID wDeviceID, UINT uStatus);
mcidrivernotify mciDriverNotify_Real;
BOOL APIENTRY mciDriverNotify(HANDLE hwndCallback, MCIDEVICEID wDeviceID, UINT uStatus)
{
	return (mciDriverNotify_Real)(hwndCallback, wDeviceID, uStatus);
}

typedef UINT(WINAPI* mcidriveryield)(MCIDEVICEID wDeviceID);
mcidriveryield mciDriverYield_Real;
UINT APIENTRY mciDriverYield(MCIDEVICEID wDeviceID)
{
	return (mciDriverYield_Real)(wDeviceID);
}

typedef BOOL(WINAPI* mcifreecommandresource)(UINT wTable);
mcifreecommandresource mciFreeCommandResource_Real;
BOOL APIENTRY mciFreeCommandResource(UINT wTable)
{
	return (mciFreeCommandResource_Real)(wTable);
}


typedef HTASK(WINAPI* mcigetcreatortask)(MCIDEVICEID mciId);
mcigetcreatortask mciGetCreatorTask_Real;
HTASK WINAPI mciGetCreatorTask(MCIDEVICEID mciId)
{
	return (mciGetCreatorTask_Real)(mciId);
}

typedef MCIDEVICEID(WINAPI* mcigetdeviceida)(LPCSTR pszDevice);
mcigetdeviceida mciGetDeviceIDA_Real;
MCIDEVICEID WINAPI mciGetDeviceIDA(LPCSTR pszDevice)
{
	return (mciGetDeviceIDA_Real)(pszDevice);
}


typedef MCIDEVICEID(WINAPI* mcigetdeviceidfromelementida)(DWORD dwElementID, LPCSTR lpstrType);
mcigetdeviceidfromelementida mciGetDeviceIDFromElementIDA_Real;
MCIDEVICEID WINAPI mciGetDeviceIDFromElementIDA(DWORD dwElementID, LPCSTR lpstrType)
{
	return (mciGetDeviceIDFromElementIDA_Real)(dwElementID, lpstrType);
}

typedef MCIDEVICEID(WINAPI* mcigetdeviceidfromelementidw)(DWORD dwElementID, LPCWSTR lpstrType);
mcigetdeviceidfromelementidw mciGetDeviceIDFromElementIDW_Real;
MCIDEVICEID WINAPI mciGetDeviceIDFromElementIDW(DWORD dwElementID, LPCWSTR lpstrType)
{
	return (mciGetDeviceIDFromElementIDW_Real)(dwElementID, lpstrType);
}

typedef MCIDEVICEID(WINAPI* mcigetdeviceidw)(LPCWSTR pszDevice);
mcigetdeviceidw mciGetDeviceIDW_Real;
MCIDEVICEID WINAPI mciGetDeviceIDW(LPCWSTR pszDevice)
{
	return (mciGetDeviceIDW_Real)(pszDevice);
}

typedef DWORD_PTR(WINAPI* mcigetdriverdata)(MCIDEVICEID wDeviceID);
mcigetdriverdata mciGetDriverData_Real;
DWORD_PTR APIENTRY mciGetDriverData(MCIDEVICEID wDeviceID)
{
	return (mciGetDriverData_Real)(wDeviceID);
}

typedef BOOL(WINAPI* mcigeterrorstringa)(MCIERROR mcierr, LPSTR pszText, UINT cchText);
mcigeterrorstringa mciGetErrorStringA_Real;
BOOL WINAPI mciGetErrorStringA(MCIERROR mcierr, LPSTR pszText, UINT cchText)
{
	return (mciGetErrorStringA_Real)(mcierr, pszText, cchText);
}

typedef BOOL(WINAPI* mcigeterrorstringw)(MCIERROR mcierr, LPWSTR pszText, UINT cchText);
mcigeterrorstringw mciGetErrorStringW_Real;
BOOL WINAPI mciGetErrorStringW(MCIERROR mcierr, LPWSTR pszText, UINT cchText)
{
	return (mciGetErrorStringW_Real)(mcierr, pszText, cchText);
}

typedef YIELDPROC(WINAPI* mcigetyieldproc)(MCIDEVICEID mciId, LPDWORD pdwYieldData);
mcigetyieldproc mciGetYieldProc_Real;
YIELDPROC WINAPI mciGetYieldProc(MCIDEVICEID mciId, LPDWORD pdwYieldData)
{
	return (mciGetYieldProc_Real)(mciId, pdwYieldData);
}

typedef UINT(WINAPI* mciloadcommandresource)(HANDLE hInstance, LPCWSTR lpResName, UINT wType);
mciloadcommandresource mciLoadCommandResource_Real;
UINT APIENTRY mciLoadCommandResource(HANDLE hInstance, LPCWSTR lpResName, UINT wType)
{
	return (mciLoadCommandResource_Real)(hInstance, lpResName, wType);
}

typedef MCIERROR(WINAPI* mcisendcommanda)(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
mcisendcommanda mciSendCommandA_Real;
MCIERROR WINAPI mciSendCommandA(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	return (mciSendCommandA_Real)(mciId, uMsg, dwParam1, dwParam2);
}

typedef MCIERROR(WINAPI* mcisendcommandw)(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
mcisendcommandw mciSendCommandW_Real;
MCIERROR WINAPI mciSendCommandW(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	return (mciSendCommandW_Real)(mciId, uMsg, dwParam1, dwParam2);
}

typedef MCIERROR(WINAPI* mcisendstringa)(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback);
mcisendstringa mciSendStringA_Real;
MCIERROR WINAPI mciSendStringA(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback)
{
	return (mciSendStringA_Real)(lpstrCommand, lpstrReturnString, uReturnLength, hwndCallback);
}

typedef MCIERROR(WINAPI* mcisendstringw)(LPCWSTR lpstrCommand, LPWSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback);
mcisendstringw mciSendStringW_Real;
MCIERROR WINAPI mciSendStringW(LPCWSTR lpstrCommand, LPWSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback)
{
	return (mciSendStringW_Real)(lpstrCommand, lpstrReturnString, uReturnLength, hwndCallback);
}

void setup_winmm() {
	char path[MAX_PATH]{};
	CopyMemory(path + GetSystemDirectory(path, MAX_PATH - 12), "\\winmm.dll", 12);
	winmm_dll = LoadLibrary(path);

	if (winmm_dll == NULL)
		MsgBoxExit(MB_ICONERROR, "Exiting", "Source winmm.dll missing");

	mciExecute_Real = (mciexecute)GetProcAddress(winmm_dll, "mciExecute");
	CloseDriver_Real = (closedriver)GetProcAddress(winmm_dll, "CloseDriver");
	DefDriverProc_Real = (defdriverproc)GetProcAddress(winmm_dll, "DefDriverProc");
	DriverCallback_Real = (drivercallback)GetProcAddress(winmm_dll, "DriverCallback");
	DrvGetModuleHandle_Real = (drvgetmodulehandle)GetProcAddress(winmm_dll, "DrvGetModuleHandle");
	GetDriverModuleHandle_Real = (getdrivermodulehandle)GetProcAddress(winmm_dll, "GetDriverModuleHandle");
	OpenDriver_Real = (opendriver)GetProcAddress(winmm_dll, "OpenDriver");
	PlaySound_Real = (playsound)GetProcAddress(winmm_dll, "PlaySound");
	PlaySoundA_Real = (playsounda)GetProcAddress(winmm_dll, "PlaySoundA");
	PlaySoundW_Real = (playsoundw)GetProcAddress(winmm_dll, "PlaySoundW");
	SendDriverMessage_Real = (senddrivermessage)GetProcAddress(winmm_dll, "SendDriverMessage");
	WOWAppExit_Real = (wowappexit)GetProcAddress(winmm_dll, "WOWAppExit");
	auxGetDevCapsA_Real = (auxgetdevcapsa)GetProcAddress(winmm_dll, "auxGetDevCapsA");
	auxGetDevCapsW_Real = (auxgetdevcapsw)GetProcAddress(winmm_dll, "auxGetDevCapsW");
	auxGetNumDevs_Real = (auxgetnumdevs)GetProcAddress(winmm_dll, "auxGetNumDevs");
	auxGetVolume_Real = (auxgetvolume)GetProcAddress(winmm_dll, "auxGetVolume");
	auxOutMessage_Real = (auxoutmessage)GetProcAddress(winmm_dll, "auxOutMessage");
	auxSetVolume_Real = (auxsetvolume)GetProcAddress(winmm_dll, "auxSetVolume");
	joyConfigChanged_Real = (joyconfigchanged)GetProcAddress(winmm_dll, "joyConfigChanged");
	joyGetDevCapsA_Real = (joygetdevcapsa)GetProcAddress(winmm_dll, "joyGetDevCapsA");
	joyGetDevCapsW_Real = (joygetdevcapsw)GetProcAddress(winmm_dll, "joyGetDevCapsW");
	joyGetNumDevs_Real = (joygetnumdevs)GetProcAddress(winmm_dll, "joyGetNumDevs");
	joyGetPos_Real = (joygetpos)GetProcAddress(winmm_dll, "joyGetPos");
	joyGetPosEx_Real = (joygetposex)GetProcAddress(winmm_dll, "joyGetPosEx");
	joyGetThreshold_Real = (joygetthreshold)GetProcAddress(winmm_dll, "joyGetThreshold");
	joyReleaseCapture_Real = (joyreleasecapture)GetProcAddress(winmm_dll, "joyReleaseCapture");
	joySetCapture_Real = (joysetcapture)GetProcAddress(winmm_dll, "joySetCapture");
	joySetThreshold_Real = (joysetthreshold)GetProcAddress(winmm_dll, "joySetThreshold");
	mciDriverNotify_Real = (mcidrivernotify)GetProcAddress(winmm_dll, "mciDriverNotify");
	mciDriverYield_Real = (mcidriveryield)GetProcAddress(winmm_dll, "mciDriverYield");
	mciFreeCommandResource_Real = (mcifreecommandresource)GetProcAddress(winmm_dll, "mciFreeCommandResource");
	mciGetCreatorTask_Real = (mcigetcreatortask)GetProcAddress(winmm_dll, "mciGetCreatorTask");
	mciGetDeviceIDA_Real = (mcigetdeviceida)GetProcAddress(winmm_dll, "mciGetDeviceIDA");
	mciGetDeviceIDFromElementIDA_Real = (mcigetdeviceidfromelementida)GetProcAddress(winmm_dll, "mciGetDeviceIDFromElementIDA");
	mciGetDeviceIDFromElementIDW_Real = (mcigetdeviceidfromelementidw)GetProcAddress(winmm_dll, "mciGetDeviceIDFromElementIDW");
	mciGetDeviceIDW_Real = (mcigetdeviceidw)GetProcAddress(winmm_dll, "mciGetDeviceIDW");
	mciGetDriverData_Real = (mcigetdriverdata)GetProcAddress(winmm_dll, "mciGetDriverData");
	mciGetErrorStringA_Real = (mcigeterrorstringa)GetProcAddress(winmm_dll, "mciGetErrorStringA");
	mciGetErrorStringW_Real = (mcigeterrorstringw)GetProcAddress(winmm_dll, "mciGetErrorStringW");
	mciGetYieldProc_Real = (mcigetyieldproc)GetProcAddress(winmm_dll, "mciGetYieldProc");
	mciLoadCommandResource_Real = (mciloadcommandresource)GetProcAddress(winmm_dll, "mciLoadCommandResource");
	mciSendCommandA_Real = (mcisendcommanda)GetProcAddress(winmm_dll, "mciSendCommandA");
	mciSendCommandW_Real = (mcisendcommandw)GetProcAddress(winmm_dll, "mciSendCommandW");
	mciSendStringA_Real = (mcisendstringa)GetProcAddress(winmm_dll, "mciSendStringA");
	mciSendStringW_Real = (mcisendstringw)GetProcAddress(winmm_dll, "mciSendStringW");

	/*
	mciSetDriverData_Real = (mciSetDriverData)GetProcAddress(winmm_dll, "mciSetDriverData");
	mciSetYieldProc_Real = (mciSetYieldProc)GetProcAddress(winmm_dll, "mciSetYieldProc");
	midiConnect_Real = (midiConnect)GetProcAddress(winmm_dll, "midiConnect");
	midiDisconnect_Real = (midiDisconnect)GetProcAddress(winmm_dll, "midiDisconnect");
	midiInAddBuffer_Real = (midiInAddBuffer)GetProcAddress(winmm_dll, "midiInAddBuffer");
	midiInClose_Real = (midiInClose)GetProcAddress(winmm_dll, "midiInClose");
	midiInGetDevCapsA_Real = (midiInGetDevCapsA)GetProcAddress(winmm_dll, "midiInGetDevCapsA");
	midiInGetDevCapsW_Real = (midiInGetDevCapsW)GetProcAddress(winmm_dll, "midiInGetDevCapsW");
	midiInGetErrorTextA_Real = (midiInGetErrorTextA)GetProcAddress(winmm_dll, "midiInGetErrorTextA");
	midiInGetErrorTextW_Real = (midiInGetErrorTextW)GetProcAddress(winmm_dll, "midiInGetErrorTextW");
	midiInGetID_Real = (midiInGetID)GetProcAddress(winmm_dll, "midiInGetID");
	midiInGetNumDevs_Real = (midiInGetNumDevs)GetProcAddress(winmm_dll, "midiInGetNumDevs");
	midiInMessage_Real = (midiInMessage)GetProcAddress(winmm_dll, "midiInMessage");
	midiInOpen_Real = (midiInOpen)GetProcAddress(winmm_dll, "midiInOpen");
	midiInPrepareHeader_Real = (midiInPrepareHeader)GetProcAddress(winmm_dll, "midiInPrepareHeader");
	midiInReset_Real = (midiInReset)GetProcAddress(winmm_dll, "midiInReset");
	midiInStart_Real = (midiInStart)GetProcAddress(winmm_dll, "midiInStart");
	midiInStop_Real = (midiInSto)GetProcAddress(winmm_dll, "midiInStop");
	midiInUnprepareHeader_Real "= ()GetProcAddress(winmm_dll, "midiInUnprepareHeader");
	midiOutCacheDrumPatches_Real, l = (midiOutCacheDrumPatches)GetProcAddress(winmm_dll, "midiOutCacheDrumPatches");
	midiOutCachePatches_Real = (midiOutCachePatches)GetProcAddress(winmm_dll, "midiOutCachePatches");
	midiOutClose_Real = (midiOutClose)GetProcAddress(winmm_dll, "midiOutClose");
	midiOutGetDevCapsA_Real = (midiOutGetDevCapsA)GetProcAddress(winmm_dll, "midiOutGetDevCapsA");
	midiOutGetDevCapsW_Real = (midiOutGetDevCaps)GetProcAddress(winmm_dll, "midiOutGetDevCapsW");
	midiOutGetErrorTextA_Real = (midiOutGetErrorText)GetProcAddress(winmm_dll, "midiOutGetErrorTextA");
	midiOutGetErrorTextW_Real = (midiOutGetErrorTextW)GetProcAddress(winmm_dll, "midiOutGetErrorTextW");
	midiOutGetID_Real = (midiOutGetID)GetProcAddress(winmm_dll, "midiOutGetID");
	midiOutGetNumDevs_Real = (midiOutGetNumDevs)GetProcAddress(winmm_dll, "midiOutGetNumDevs");
	midiOutGetVolume_Real = (midiOutGetVolume)GetProcAddress(winmm_dll, "midiOutGetVolume");
	midiOutLongMsg_Real = (midiOutLongMsg)GetProcAddress(winmm_dll, "midiOutLongMsg");
	midiOutMessage_Real = (midiOutMessage)GetProcAddress(winmm_dll, "midiOutMessage");
	midiOutOpen_Real = (midiOutOpe)GetProcAddress(winmm_dll, "midiOutOpen");
	midiOutPrepareHeader_Real = (midiOutPrepareHeader)GetProcAddress(winmm_dll, "midiOutPrepareHeader");
	midiOutReset_Real = (midiOutReset)GetProcAddress(winmm_dll, "midiOutReset");
	midiOutSetVolume_Real = (midiOutSetVolume)GetProcAddress(winmm_dll, "midiOutSetVolume");
	midiOutShortMsg_Real = (midiOutShortMsg)GetProcAddress(winmm_dll, "midiOutShortMsg");
	midiOutUnprepareHeader_Real = (midiOutUnprepareHeader)GetProcAddress(winmm_dll, "midiOutUnprepareHeader");
	midiStreamClose_Real = (midiStreamClose)GetProcAddress(winmm_dll, "midiStreamClose");
	midiStreamOpen_Real = (midiStreamOpen)GetProcAddress(winmm_dll, "midiStreamOpen");
	midiStreamOut_Real = (midiStreamOut)GetProcAddress(winmm_dll, "midiStreamOut");
	midiStreamPause_Real = (midiStreamPause)GetProcAddress(winmm_dll, "midiStreamPause");
	midiStreamPosition_Real = (midiStreamPosition)GetProcAddress(winmm_dll, "midiStreamPosition");
	midiStreamProperty_Real = (midiStreamProperty)GetProcAddress(winmm_dll, "midiStreamProperty");
	midiStreamRestart_Real = (midiStreamRestart)GetProcAddress(winmm_dll, "midiStreamRestart");
	midiStreamStop_Real = (midiStreamStop)GetProcAddress(winmm_dll, "midiStreamStop");
	mixerClose_Real = (mixerClose)GetProcAddress(winmm_dll, "mixerClose");
	mixerGetControlDetailsA_Real = (mixerGetControlDetailsA)GetProcAddress(winmm_dll, "mixerGetControlDetailsA");
	mixerGetControlDetailsW_Real = (mixerGetControlDetailsW)GetProcAddress(winmm_dll, "mixerGetControlDetailsW");
	mixerGetDevCapsA_Real = (mixerGetDevCapsA)GetProcAddress(winmm_dll, "mixerGetDevCapsA");
	mixerGetDevCapsW_Real = (mixerGetDevCapsW)GetProcAddress(winmm_dll, "mixerGetDevCapsW");
	mixerGetID_Real = (mixerGetI)GetProcAddress(winmm_dll, "mixerGetID");
	mixerGetLineControlsA_Real = (mixerGetLineControls)GetProcAddress(winmm_dll, "mixerGetLineControlsA");
	mixerGetLineControlsW_Real = (mixerGetLineControlsW)GetProcAddress(winmm_dll, "mixerGetLineControlsW");
	mixerGetLineInfoA_Real = (mixerGetLineInfoA)GetProcAddress(winmm_dll, "mixerGetLineInfoA");
	mixerGetLineInfoW_Real = (mixerGetLineInfoW)GetProcAddress(winmm_dll, "mixerGetLineInfoW");
	mixerGetNumDevs_Real = (mixerGetNumDevs)GetProcAddress(winmm_dll, "mixerGetNumDevs");
	mixerMessage_Real = (mixerMessage)GetProcAddress(winmm_dll, "mixerMessage");
	mixerOpen_Real = (mixerOpen)GetProcAddress(winmm_dll, "mixerOpen");
	mixerSetControlDetails_Real = (mixerSetControlDetails)GetProcAddress(winmm_dll, "mixerSetControlDetails");
	mmDrvInstall_Real = (mmDrvInstall)GetProcAddress(winmm_dll, "mmDrvInstall");
	mmGetCurrentTask_Real = (mmGetCurrentTask)GetProcAddress(winmm_dll, "mmGetCurrentTask");
	mmTaskBlock_Real = (mmTaskBlock)GetProcAddress(winmm_dll, "mmTaskBlock");
	mmTaskCreate_Real = (mmTaskCreate)GetProcAddress(winmm_dll, "mmTaskCreate");
	mmTaskSignal_Real = (mmTaskSignal)GetProcAddress(winmm_dll, "mmTaskSignal");
	mmTaskYield_Real = (mmTaskYield)GetProcAddress(winmm_dll, "mmTaskYield");
	mmioAdvance_Real = (mmioAdvance)GetProcAddress(winmm_dll, "mmioAdvance");
	mmioAscend_Real = (mmioAscend)GetProcAddress(winmm_dll, "mmioAscend");
	mmioClose_Real = (mmioClose)GetProcAddress(winmm_dll, "mmioClose");
	mmioCreateChunk_Real = (mmioCreateChunk)GetProcAddress(winmm_dll, "mmioCreateChunk");
	mmioDescend_Real = (mmioDescend)GetProcAddress(winmm_dll, "mmioDescend");
	mmioFlush_Real = (mmioFlush)GetProcAddress(winmm_dll, "mmioFlush");
	mmioGetInfo_Real = (mmioGetInfo)GetProcAddress(winmm_dll, "mmioGetInfo");
	mmioInstallIOProcA_Real = (mmioInstallIOProcA)GetProcAddress(winmm_dll, "mmioInstallIOProcA");
	mmioInstallIOProcW_Real = (mmioInstallIOProcW)GetProcAddress(winmm_dll, "mmioInstallIOProcW");
	mmioOpenA_Real = (mmioOpenA)GetProcAddress(winmm_dll, "mmioOpenA");
	mmioOpenW_Real = (mmioOpenW)GetProcAddress(winmm_dll, "mmioOpenW");
	mmioRead_Real = (mmioRead)GetProcAddress(winmm_dll, "mmioRead");
	mmioRenameA_Real = (mmioRenameA)GetProcAddress(winmm_dll, "mmioRenameA");
	mmioRenameW_Real = (mmioRenameW)GetProcAddress(winmm_dll, "mmioRenameW");
	mmioSeek_Real = (mmioSeek)GetProcAddress(winmm_dll, "mmioSeek");
	mmioSendMessage_Real = (mmioSendMessage)GetProcAddress(winmm_dll, "mmioSendMessage");
	mmioSetBuffer_Real = (mmioSetBuffer)GetProcAddress(winmm_dll, "mmioSetBuffer");
	mmioSetInfo_Real = (mmioSetInfo)GetProcAddress(winmm_dll, "mmioSetInfo");
	mmioStringToFOURCCA_Real = (mmioStringToFOURCCA)GetProcAddress(winmm_dll, "mmioStringToFOURCCA");
	mmioStringToFOURCCW_Real = (mmioStringToFOURCCW)GetProcAddress(winmm_dll, "mmioStringToFOURCCW");
	mmioWrite_Real = (mmioWrite)GetProcAddress(winmm_dll, "mmioWrite");
	mmsystemGetVersion_Real = (mmsystemGetVersion)GetProcAddress(winmm_dll, "mmsystemGetVersion");
	sndPlaySoundA_Real = (sndPlaySoundA)GetProcAddress(winmm_dll, "sndPlaySoundA");
	sndPlaySoundW_Real = (sndPlaySoundW)GetProcAddress(winmm_dll, "sndPlaySoundW");
	timeBeginPeriod_Real = (timeBeginPeriod)GetProcAddress(winmm_dll, "timeBeginPeriod");
	timeEndPeriod_Real = (timeEndPeriod)GetProcAddress(winmm_dll, "timeEndPeriod");
	timeGetDevCaps_Real = (timeGetDevCaps)GetProcAddress(winmm_dll, "timeGetDevCaps");
	timeGetSystemTime_Real = (timeGetSystemTime)GetProcAddress(winmm_dll, "timeGetSystemTime");
	timeGetTime_Real = (timeGetTime)GetProcAddress(winmm_dll, "timeGetTime");
	timeKillEvent_Real = (timeKillEvent)GetProcAddress(winmm_dll, "timeKillEvent");
	timeSetEvent_Real = (timeSetEvent)GetProcAddress(winmm_dll, "timeSetEvent");
	waveInAddBuffer_Real = (waveInAddBuffer)GetProcAddress(winmm_dll, "waveInAddBuffer");
	waveInClose_Real = (waveInClose)GetProcAddress(winmm_dll, "waveInClose");
	waveInGetDevCapsA_Real = (waveInGetDevCapsA)GetProcAddress(winmm_dll, "waveInGetDevCapsA");
	waveInGetDevCapsW_Real = (waveInGetDevCapsW)GetProcAddress(winmm_dll, "waveInGetDevCapsW");
	waveInGetErrorTextA_Real = (waveInGetErrorTextA)GetProcAddress(winmm_dll, "waveInGetErrorTextA");
	waveInGetErrorTextW_Real = (waveInGetErrorTextW)GetProcAddress(winmm_dll, "waveInGetErrorTextW");
	waveInGetID_Real = (waveInGetID)GetProcAddress(winmm_dll, "waveInGetID");
	waveInGetNumDevs_Real = (waveInGetNumDevs)GetProcAddress(winmm_dll, "waveInGetNumDevs");
	waveInGetPosition_Real = (waveInGetPosition)GetProcAddress(winmm_dll, "waveInGetPosition");
	waveInMessage_Real = (waveInMessage)GetProcAddress(winmm_dll, "waveInMessage");
	waveInOpen_Real = (waveInOpen)GetProcAddress(winmm_dll, "waveInOpen");
	waveInPrepareHeader_Real = (waveInPrepareHeader)GetProcAddress(winmm_dll, "waveInPrepareHeader");
	waveInReset_Real = (waveInReset)GetProcAddress(winmm_dll, "waveInReset");
	waveInStart_Real = (waveInStart)GetProcAddress(winmm_dll, "waveInStart");
	waveInStop_Real = (waveInStop)GetProcAddress(winmm_dll, "waveInStop");
	waveInUnprepareHeader_Real = (waveInUnprepareHeader)GetProcAddress(winmm_dll, "waveInUnprepareHeader");
	waveOutBreakLoop_Real = (waveOutBreakLoop)GetProcAddress(winmm_dll, "waveOutBreakLoop");
	waveOutClose_Real = (waveOutClose)GetProcAddress(winmm_dll, "waveOutClose");
	waveOutGetDevCapsA_Real = (waveOutGetDevCapsA)GetProcAddress(winmm_dll, "waveOutGetDevCapsA");
	waveOutGetDevCapsW_Real = (waveOutGetDevCaps)GetProcAddress(winmm_dll, "waveOutGetDevCapsW");
	waveOutGetErrorTextA_Real = (waveOutGetErrorText)GetProcAddress(winmm_dll, "waveOutGetErrorTextA");
	waveOutGetErrorTextW_Real = (waveOutGetErrorTextW)GetProcAddress(winmm_dll, "waveOutGetErrorTextW");
	waveOutGetID_Real = (waveOutGetID)GetProcAddress(winmm_dll, "waveOutGetID");
	waveOutGetNumDevs_Real = (waveOutGetNumDevs)GetProcAddress(winmm_dll, "waveOutGetNumDevs");
	waveOutGetPitch_Real = (waveOutGetPitch)GetProcAddress(winmm_dll, "waveOutGetPitch");
	waveOutGetPlaybackRate_Real = (waveOutGetPlaybackRate)GetProcAddress(winmm_dll, "waveOutGetPlaybackRate");
	waveOutGetPosition_Real = (waveOutGetPosition)GetProcAddress(winmm_dll, "waveOutGetPosition");
	waveOutGetVolume_Real = (waveOutGetVolume)GetProcAddress(winmm_dll, "waveOutGetVolume");
	waveOutMessage_Real = (waveOutMessage)GetProcAddress(winmm_dll, "waveOutMessage");
	waveOutOpen_Real = (waveOutOpen)GetProcAddress(winmm_dll, "waveOutOpen");
	waveOutPause_Real = (waveOutPaus)GetProcAddress(winmm_dll, "waveOutPause");
	waveOutPrepareHeader_Real = (waveOutPrepareHeader)GetProcAddress(winmm_dll, "waveOutPrepareHeader");
	waveOutReset_Real = (waveOutReset)GetProcAddress(winmm_dll, "waveOutReset");
	waveOutRestart_Real = (waveOutRestart)GetProcAddress(winmm_dll, "waveOutRestart");
	waveOutSetPitch_Real = (waveOutSetPitch)GetProcAddress(winmm_dll, "waveOutSetPitch");
	waveOutSetPlaybackRate_Real = (waveOutSetPlaybackRate)GetProcAddress(winmm_dll, "waveOutSetPlaybackRate");
	waveOutSetVolume_Real = (waveOutSetVolume)GetProcAddress(winmm_dll, "waveOutSetVolume");
	waveOutUnprepareHeader_Real = (waveOutUnprepareHeader)GetProcAddress(winmm_dll, "waveOutUnprepareHeader");
	waveOutWrite_Real = (waveOutWrite)GetProcAddress(winmm_dll, "waveOutWrit");
	*/
}

void free_winmm() {
	(void)FreeLibrary(winmm_dll);
}