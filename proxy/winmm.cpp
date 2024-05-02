#include "winmm.h"
#include "../common.h"

HMODULE winmm_dll;

typedef VOID(TASKCALLBACK) (DWORD_PTR dwInst);
typedef TASKCALLBACK* LPTASKCALLBACK;

using t_mciExecute = bool (WINAPI*)(LPCSTR pszCommand);
using t_CloseDriver = LRESULT(WINAPI*)(HDRVR hDriver, LPARAM lParam1, LPARAM lParam2);
using t_DefDriverProc = LRESULT(WINAPI*)(DWORD_PTR dwDriverIdentifier, HDRVR hdrvr, UINT uMsg, LPARAM lParam1, LPARAM lParam2);
using t_DriverCallback = bool (APIENTRY*)(DWORD_PTR dwCallback, DWORD dwFlags, HDRVR hDevice, DWORD dwMsg, DWORD_PTR dwUser, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
using t_DrvGetModuleHandle = HMODULE(WINAPI*)(HDRVR hDriver);
using t_GetDriverModuleHandle = HMODULE(WINAPI*)(HDRVR hDriver);
using t_OpenDriver = HDRVR(WINAPI*)(LPCWSTR szDriverName, LPCWSTR szSectionName, LPARAM lParam2);
using t_PlaySound = bool (WINAPI*)(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound);
using t_PlaySoundA = bool (WINAPI*)(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound);
using t_PlaySoundW = bool (WINAPI*)(LPCWSTR pszSound, HMODULE hmod, DWORD fdwSound);
using t_SendDriverMessage = LRESULT(WINAPI*)(HDRVR hDriver, UINT message, LPARAM lParam1, LPARAM lParam2);
using t_WOWAppExit = void (WINAPI*)();
using t_auxGetDevCapsA = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPAUXCAPSA pac, UINT cbac);
using t_auxGetDevCapsW = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPAUXCAPSW pac, UINT cbac);
using t_auxGetNumDevs = UINT(WINAPI*)();
using t_auxGetVolume = MMRESULT(WINAPI*)(UINT uDeviceID, LPDWORD pdwVolume);
using t_auxOutMessage = MMRESULT(WINAPI*)(UINT uDeviceID, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
using t_auxSetVolume = MMRESULT(WINAPI*)(UINT uDeviceID, DWORD dwVolume);
using t_joyConfigChanged = MMRESULT(WINAPI*)(DWORD dwFlags);
using t_joyGetDevCapsA = MMRESULT(WINAPI*)(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc);
using t_joyGetDevCapsW = MMRESULT(WINAPI*)(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc);
using t_joyGetNumDevs = UINT(WINAPI*)();
using t_joyGetPos = MMRESULT(WINAPI*)(UINT uJoyID, LPJOYINFO pji);
using t_joyGetPosEx = MMRESULT(WINAPI*)(UINT uJoyID, LPJOYINFOEX pji);
using t_joyGetThreshold = MMRESULT(WINAPI*)(UINT uJoyID, LPUINT puThreshold);
using t_joyReleaseCapture = MMRESULT(WINAPI*)(UINT uJoyID);
using t_joySetCapture = MMRESULT(WINAPI*)(HWND hwnd, UINT uJoyID, UINT uPeriod, bool fChanged);
using t_joySetThreshold = MMRESULT(WINAPI*)(UINT uJoyID, UINT uThreshold);
using t_mciDriverNotify = bool (APIENTRY*)(HANDLE hwndCallback, MCIDEVICEID wDeviceID, UINT uStatus);
using t_mciDriverYield = UINT(APIENTRY*)(MCIDEVICEID wDeviceID);
using t_mciFreeCommandResource = bool (APIENTRY*)(UINT wTable);
using t_mciGetCreatorTask = HTASK(WINAPI*)(MCIDEVICEID mciId);
using t_mciGetDeviceIDA = MCIDEVICEID(WINAPI*)(LPCSTR pszDevice);
using t_mciGetDeviceIDFromElementIDA = MCIDEVICEID(WINAPI*)(DWORD dwElementID, LPCSTR lpstrType);
using t_mciGetDeviceIDFromElementIDW = MCIDEVICEID(WINAPI*)(DWORD dwElementID, LPCWSTR lpstrType);
using t_mciGetDeviceIDW = MCIDEVICEID(WINAPI*)(LPCWSTR pszDevice);
using t_mciGetDriverData = DWORD_PTR(APIENTRY*)(MCIDEVICEID wDeviceID);
using t_mciGetErrorStringA = bool (WINAPI*)(MCIERROR mcierr, LPSTR pszText, UINT cchText);
using t_mciGetErrorStringW = bool (WINAPI*)(MCIERROR mcierr, LPWSTR pszText, UINT cchText);
using t_mciGetYieldProc = YIELDPROC(WINAPI*)(MCIDEVICEID mciId, LPDWORD pdwYieldData);
using t_mciLoadCommandResource = UINT(APIENTRY*)(HANDLE hInstance, LPCWSTR lpResName, UINT wType);
using t_mciSendCommandA = MCIERROR(WINAPI*)(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
using t_mciSendCommandW = MCIERROR(WINAPI*)(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
using t_mciSendStringA = MCIERROR(WINAPI*)(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback);
using t_mciSendStringW = MCIERROR(WINAPI*)(LPCWSTR lpstrCommand, LPWSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback);
using t_mciSetDriverData = bool (APIENTRY*)(MCIDEVICEID wDeviceID, DWORD_PTR dwData);
using t_mciSetYieldProc = bool (WINAPI*)(MCIDEVICEID mciId, YIELDPROC fpYieldProc, DWORD dwYieldData);
using t_midiConnect = MMRESULT(WINAPI*)(HMIDI hmi, HMIDIOUT hmo, LPVOID pReserved);
using t_midiDisconnect = MMRESULT(WINAPI*)(HMIDI hmi, HMIDIOUT hmo, LPVOID pReserved);
using t_midiInAddBuffer = MMRESULT(WINAPI*)(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh);
using t_midiInClose = MMRESULT(WINAPI*)(HMIDIIN hmi);
using t_midiInGetDevCapsA = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPMIDIINCAPSA pmic, UINT cbmic);
using t_midiInGetDevCapsW = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPMIDIINCAPSW pmic, UINT cbmic);
using t_midiInGetErrorTextA = MMRESULT(WINAPI*)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
using t_midiInGetErrorTextW = MMRESULT(WINAPI*)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
using t_midiInGetID = MMRESULT(WINAPI*)(HMIDIIN hmi, LPUINT puDeviceID);
using t_midiInGetNumDevs = UINT(WINAPI*)();
using t_midiInMessage = MMRESULT(WINAPI*)(HMIDIIN hmi, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
using t_midiInOpen = MMRESULT(WINAPI*)(LPHMIDIIN phmi, UINT uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
using t_midiInPrepareHeader = MMRESULT(WINAPI*)(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh);
using t_midiInReset = MMRESULT(WINAPI*)(HMIDIIN hmi);
using t_midiInStart = MMRESULT(WINAPI*)(HMIDIIN hmi);
using t_midiInStop = MMRESULT(WINAPI*)(HMIDIIN hmi);
using t_midiInUnprepareHeader = MMRESULT(WINAPI*)(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh);
using t_midiOutCacheDrumPatches = MMRESULT(WINAPI*)(HMIDIOUT hmo, UINT uPatch, LPWORD pwkya, UINT fuCache);
using t_midiOutCachePatches = MMRESULT(WINAPI*)(HMIDIOUT hmo, UINT uBank, LPWORD pwpa, UINT fuCache);
using t_midiOutClose = MMRESULT(WINAPI*)(HMIDIOUT hmo);
using t_midiOutGetDevCapsA = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPMIDIOUTCAPSA pmoc, UINT cbmoc);
using t_midiOutGetDevCapsW = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPMIDIOUTCAPSW pmoc, UINT cbmoc);
using t_midiOutGetErrorTextA = MMRESULT(WINAPI*)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
using t_midiOutGetErrorTextW = MMRESULT(WINAPI*)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
using t_midiOutGetID = MMRESULT(WINAPI*)(HMIDIOUT hmo, LPUINT puDeviceID);
using t_midiOutGetNumDevs = UINT(WINAPI*)();
using t_midiOutGetVolume = MMRESULT(WINAPI*)(HMIDIOUT hmo, LPDWORD pdwVolume);
using t_midiOutLongMsg = MMRESULT(WINAPI*)(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh);
using t_midiOutMessage = MMRESULT(WINAPI*)(HMIDIOUT hmo, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
using t_midiOutOpen = MMRESULT(WINAPI*)(LPHMIDIOUT phmo, UINT uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
using t_midiOutPrepareHeader = MMRESULT(WINAPI*)(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh);
using t_midiOutReset = MMRESULT(WINAPI*)(HMIDIOUT hmo);
using t_midiOutSetVolume = MMRESULT(WINAPI*)(HMIDIOUT hmo, DWORD dwVolume);
using t_midiOutShortMsg = MMRESULT(WINAPI*)(HMIDIOUT hmo, DWORD dwMsg);
using t_midiOutUnprepareHeader = MMRESULT(WINAPI*)(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh);
using t_midiStreamClose = MMRESULT(WINAPI*)(HMIDISTRM hms);
using t_midiStreamOpen = MMRESULT(WINAPI*)(LPHMIDISTRM phms, LPUINT puDeviceID, DWORD cMidi, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
using t_midiStreamOut = MMRESULT(WINAPI*)(HMIDISTRM hms, LPMIDIHDR pmh, UINT cbmh);
using t_midiStreamPause = MMRESULT(WINAPI*)(HMIDISTRM hms);
using t_midiStreamPosition = MMRESULT(WINAPI*)(HMIDISTRM hms, LPMMTIME lpmmt, UINT cbmmt);
using t_midiStreamProperty = MMRESULT(WINAPI*)(HMIDISTRM hms, LPBYTE lppropdata, DWORD dwProperty);
using t_midiStreamRestart = MMRESULT(WINAPI*)(HMIDISTRM hms);
using t_midiStreamStop = MMRESULT(WINAPI*)(HMIDISTRM hms);
using t_mixerClose = MMRESULT(WINAPI*)(HMIXER hmx);
using t_mixerGetControlDetailsA = MMRESULT(WINAPI*)(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails);
using t_mixerGetControlDetailsW = MMRESULT(WINAPI*)(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails);
using t_mixerGetDevCapsA = MMRESULT(WINAPI*)(UINT_PTR uMxId, LPMIXERCAPSA pmxcaps, UINT cbmxcaps);
using t_mixerGetDevCapsW = MMRESULT(WINAPI*)(UINT_PTR uMxId, LPMIXERCAPSW pmxcaps, UINT cbmxcaps);
using t_mixerGetID = MMRESULT(WINAPI*)(HMIXEROBJ hmxobj, UINT FAR* puMxId, DWORD fdwId);
using t_mixerGetLineControlsA = MMRESULT(WINAPI*)(HMIXEROBJ hmxobj, LPMIXERLINECONTROLSA pmxlc, DWORD fdwControls);
using t_mixerGetLineControlsW = MMRESULT(WINAPI*)(HMIXEROBJ hmxobj, LPMIXERLINECONTROLSW pmxlc, DWORD fdwControls);
using t_mixerGetLineInfoA = MMRESULT(WINAPI*)(HMIXEROBJ hmxobj, LPMIXERLINEA pmxl, DWORD fdwInfo);
using t_mixerGetLineInfoW = MMRESULT(WINAPI*)(HMIXEROBJ hmxobj, LPMIXERLINEW pmxl, DWORD fdwInfo);
using t_mixerGetNumDevs = UINT(WINAPI*)();
using t_mixerMessage = DWORD(WINAPI*)(HMIXER hmx, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
using t_mixerOpen = MMRESULT(WINAPI*)(LPHMIXER phmx, UINT uMxId, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
using t_mixerSetControlDetails = MMRESULT(WINAPI*)(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails);
using t_mmDrvInstall = UINT(APIENTRY*)(HDRVR hDriver, LPCWSTR wszDrvEntry, DRIVERMSGPROC drvMessage, UINT wFlags);
using t_mmGetCurrentTask = DWORD(APIENTRY*)();
using t_mmTaskBlock = VOID(APIENTRY*)(DWORD h);
using t_mmTaskCreate = UINT(APIENTRY*)(LPTASKCALLBACK lpfn, HANDLE FAR* lph, DWORD_PTR dwInst);
using t_mmTaskSignal = bool (APIENTRY*)(DWORD h);
using t_mmTaskYield = VOID(APIENTRY*)();
using t_mmioAdvance = MMRESULT(WINAPI*)(HMMIO hmmio, LPMMIOINFO pmmioinfo, UINT fuAdvance);
using t_mmioAscend = MMRESULT(WINAPI*)(HMMIO hmmio, LPMMCKINFO pmmcki, UINT fuAscend);
using t_mmioClose = MMRESULT(WINAPI*)(HMMIO hmmio, UINT fuClose);
using t_mmioCreateChunk = MMRESULT(WINAPI*)(HMMIO hmmio, LPMMCKINFO pmmcki, UINT fuCreate);
using t_mmioDescend = MMRESULT(WINAPI*)(HMMIO hmmio, LPMMCKINFO pmmcki, const MMCKINFO FAR* pmmckiParent, UINT fuDescend);
using t_mmioFlush = MMRESULT(WINAPI*)(HMMIO hmmio, UINT fuFlush);
using t_mmioGetInfo = MMRESULT(WINAPI*)(HMMIO hmmio, LPMMIOINFO pmmioinfo, UINT fuInfo);
using t_mmioInstallIOProcA = LPMMIOPROC(WINAPI*)(FOURCC fccIOProc, LPMMIOPROC pIOProc, DWORD dwFlags);
using t_mmioInstallIOProcW = LPMMIOPROC(WINAPI*)(FOURCC fccIOProc, LPMMIOPROC pIOProc, DWORD dwFlags);
using t_mmioOpenA = HMMIO(WINAPI*)(LPSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen);
using t_mmioOpenW = HMMIO(WINAPI*)(LPWSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen);
using t_mmioRead = LONG(WINAPI*)(HMMIO hmmio, HPSTR pch, LONG cch);
using t_mmioRenameA = MMRESULT(WINAPI*)(LPCSTR pszFileName, LPCSTR pszNewFileName, LPCMMIOINFO pmmioinfo, DWORD fdwRename);
using t_mmioRenameW = MMRESULT(WINAPI*)(LPCWSTR pszFileName, LPCWSTR pszNewFileName, LPCMMIOINFO pmmioinfo, DWORD fdwRename);
using t_mmioSeek = LONG(WINAPI*)(HMMIO hmmio, LONG lOffset, int iOrigin);
using t_mmioSendMessage = LRESULT(WINAPI*)(HMMIO hmmio, UINT uMsg, LPARAM lParam1, LPARAM lParam2);
using t_mmioSetBuffer = MMRESULT(WINAPI*)(HMMIO hmmio, LPSTR pchBuffer, LONG cchBuffer, UINT fuBuffer);
using t_mmioSetInfo = MMRESULT(WINAPI*)(HMMIO hmmio, LPCMMIOINFO pmmioinfo, UINT fuInfo);
using t_mmioStringToFOURCCA = FOURCC(WINAPI*)(LPCSTR sz, UINT uFlags);
using t_mmioStringToFOURCCW = FOURCC(WINAPI*)(LPCWSTR sz, UINT uFlags);
using t_mmioWrite = LONG(WINAPI*)(HMMIO hmmio, const char _huge* pch, LONG cch);
using t_mmsystemGetVersion = UINT(WINAPI*)();
using t_sndPlaySoundA = bool (WINAPI*)(LPCSTR pszSound, UINT fuSound);
using t_sndPlaySoundW = bool (WINAPI*)(LPCWSTR pszSound, UINT fuSound);
using t_timeBeginPeriod = MMRESULT(WINAPI*)(UINT uPeriod);
using t_timeEndPeriod = MMRESULT(WINAPI*)(UINT uPeriod);
using t_timeGetDevCaps = MMRESULT(WINAPI*)(LPTIMECAPS ptc, UINT cbtc);
using t_timeGetSystemTime = MMRESULT(WINAPI*)(LPMMTIME pmmt, UINT cbmmt);
using t_timeGetTime = DWORD(WINAPI*)();
using t_timeKillEvent = MMRESULT(WINAPI*)(UINT uTimerID);
using t_timeSetEvent = MMRESULT(WINAPI*)(UINT uDelay, UINT uResolution, LPTIMECALLBACK fptc, DWORD_PTR dwUser, UINT fuEvent);
using t_waveInAddBuffer = MMRESULT(WINAPI*)(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
using t_waveInClose = MMRESULT(WINAPI*)(HWAVEIN hwi);
using t_waveInGetDevCapsA = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPWAVEINCAPSA pwic, UINT cbwic);
using t_waveInGetDevCapsW = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPWAVEINCAPSW pwic, UINT cbwic);
using t_waveInGetErrorTextA = MMRESULT(WINAPI*)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
using t_waveInGetErrorTextW = MMRESULT(WINAPI*)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
using t_waveInGetID = MMRESULT(WINAPI*)(HWAVEIN hwi, LPUINT puDeviceID);
using t_waveInGetNumDevs = UINT(WINAPI*)();
using t_waveInGetPosition = MMRESULT(WINAPI*)(HWAVEIN hwi, LPMMTIME pmmt, UINT cbmmt);
using t_waveInMessage = MMRESULT(WINAPI*)(HWAVEIN hwi, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
using t_waveInOpen = MMRESULT(WINAPI*)(LPHWAVEIN phwi, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
using t_waveInPrepareHeader = MMRESULT(WINAPI*)(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
using t_waveInReset = MMRESULT(WINAPI*)(HWAVEIN hwi);
using t_waveInStart = MMRESULT(WINAPI*)(HWAVEIN hwi);
using t_waveInStop = MMRESULT(WINAPI*)(HWAVEIN hwi);
using t_waveInUnprepareHeader = MMRESULT(WINAPI*)(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
using t_waveOutBreakLoop = MMRESULT(WINAPI*)(HWAVEOUT hwo);
using t_waveOutClose = MMRESULT(WINAPI*)(HWAVEOUT hwo);
using t_waveOutGetDevCapsA = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPWAVEOUTCAPSA pwoc, UINT cbwoc);
using t_waveOutGetDevCapsW = MMRESULT(WINAPI*)(UINT_PTR uDeviceID, LPWAVEOUTCAPSW pwoc, UINT cbwoc);
using t_waveOutGetErrorTextA = MMRESULT(WINAPI*)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
using t_waveOutGetErrorTextW = MMRESULT(WINAPI*)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
using t_waveOutGetID = MMRESULT(WINAPI*)(HWAVEOUT hwo, LPUINT puDeviceID);
using t_waveOutGetNumDevs = UINT(WINAPI*)();
using t_waveOutGetPitch = MMRESULT(WINAPI*)(HWAVEOUT hwo, LPDWORD pdwPitch);
using t_waveOutGetPlaybackRate = MMRESULT(WINAPI*)(HWAVEOUT hwo, LPDWORD pdwRate);
using t_waveOutGetPosition = MMRESULT(WINAPI*)(HWAVEOUT hwo, LPMMTIME pmmt, UINT cbmmt);
using t_waveOutGetVolume = MMRESULT(WINAPI*)(HWAVEOUT hwo, LPDWORD pdwVolume);
using t_waveOutMessage = MMRESULT(WINAPI*)(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
using t_waveOutOpen = MMRESULT(WINAPI*)(LPHWAVEOUT phwo, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
using t_waveOutPause = MMRESULT(WINAPI*)(HWAVEOUT hwo);
using t_waveOutPrepareHeader = MMRESULT(WINAPI*)(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
using t_waveOutReset = MMRESULT(WINAPI*)(HWAVEOUT hwo);
using t_waveOutRestart = MMRESULT(WINAPI*)(HWAVEOUT hwo);
using t_waveOutSetPitch = MMRESULT(WINAPI*)(HWAVEOUT hwo, DWORD dwPitch);
using t_waveOutSetPlaybackRate = MMRESULT(WINAPI*)(HWAVEOUT hwo, DWORD dwRate);
using t_waveOutSetVolume = MMRESULT(WINAPI*)(HWAVEOUT hwo, DWORD dwVolume);
using t_waveOutUnprepareHeader = MMRESULT(WINAPI*)(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
using t_waveOutWrite = MMRESULT(WINAPI*)(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);

t_mciExecute o_mciExecute;
t_CloseDriver o_CloseDriver;
t_DefDriverProc o_DefDriverProc;
t_DriverCallback o_DriverCallback;
t_DrvGetModuleHandle o_DrvGetModuleHandle;
t_GetDriverModuleHandle o_GetDriverModuleHandle;
t_OpenDriver o_OpenDriver;
t_PlaySound o_PlaySound;
t_PlaySoundA o_PlaySoundA;
t_PlaySoundW o_PlaySoundW;
t_SendDriverMessage o_SendDriverMessage;
t_WOWAppExit o_WOWAppExit;
t_auxGetDevCapsA o_auxGetDevCapsA;
t_auxGetDevCapsW o_auxGetDevCapsW;
t_auxGetNumDevs o_auxGetNumDevs;
t_auxGetVolume o_auxGetVolume;
t_auxOutMessage o_auxOutMessage;
t_auxSetVolume o_auxSetVolume;
t_joyConfigChanged o_joyConfigChanged;
t_joyGetDevCapsA o_joyGetDevCapsA;
t_joyGetDevCapsW o_joyGetDevCapsW;
t_joyGetNumDevs o_joyGetNumDevs;
t_joyGetPos o_joyGetPos;
t_joyGetPosEx o_joyGetPosEx;
t_joyGetThreshold o_joyGetThreshold;
t_joyReleaseCapture o_joyReleaseCapture;
t_joySetCapture o_joySetCapture;
t_joySetThreshold o_joySetThreshold;
t_mciDriverNotify o_mciDriverNotify;
t_mciDriverYield o_mciDriverYield;
t_mciFreeCommandResource o_mciFreeCommandResource;
t_mciGetCreatorTask o_mciGetCreatorTask;
t_mciGetDeviceIDA o_mciGetDeviceIDA;
t_mciGetDeviceIDFromElementIDA o_mciGetDeviceIDFromElementIDA;
t_mciGetDeviceIDFromElementIDW o_mciGetDeviceIDFromElementIDW;
t_mciGetDeviceIDW o_mciGetDeviceIDW;
t_mciGetDriverData o_mciGetDriverData;
t_mciGetErrorStringA o_mciGetErrorStringA;
t_mciGetErrorStringW o_mciGetErrorStringW;
t_mciGetYieldProc o_mciGetYieldProc;
t_mciLoadCommandResource o_mciLoadCommandResource;
t_mciSendCommandA o_mciSendCommandA;
t_mciSendCommandW o_mciSendCommandW;
t_mciSendStringA o_mciSendStringA;
t_mciSendStringW o_mciSendStringW;
t_mciSetDriverData o_mciSetDriverData;
t_mciSetYieldProc o_mciSetYieldProc;
t_midiConnect o_midiConnect;
t_midiDisconnect o_midiDisconnect;
t_midiInAddBuffer o_midiInAddBuffer;
t_midiInClose o_midiInClose;
t_midiInGetDevCapsA o_midiInGetDevCapsA;
t_midiInGetDevCapsW o_midiInGetDevCapsW;
t_midiInGetErrorTextA o_midiInGetErrorTextA;
t_midiInGetErrorTextW o_midiInGetErrorTextW;
t_midiInGetID o_midiInGetID;
t_midiInGetNumDevs o_midiInGetNumDevs;
t_midiInMessage o_midiInMessage;
t_midiInOpen o_midiInOpen;
t_midiInPrepareHeader o_midiInPrepareHeader;
t_midiInReset o_midiInReset;
t_midiInStart o_midiInStart;
t_midiInStop o_midiInStop;
t_midiInUnprepareHeader o_midiInUnprepareHeader;
t_midiOutCacheDrumPatches o_midiOutCacheDrumPatches;
t_midiOutCachePatches o_midiOutCachePatches;
t_midiOutClose o_midiOutClose;
t_midiOutGetDevCapsA o_midiOutGetDevCapsA;
t_midiOutGetDevCapsW o_midiOutGetDevCapsW;
t_midiOutGetErrorTextA o_midiOutGetErrorTextA;
t_midiOutGetErrorTextW o_midiOutGetErrorTextW;
t_midiOutGetID o_midiOutGetID;
t_midiOutGetNumDevs o_midiOutGetNumDevs;
t_midiOutGetVolume o_midiOutGetVolume;
t_midiOutLongMsg o_midiOutLongMsg;
t_midiOutMessage o_midiOutMessage;
t_midiOutOpen o_midiOutOpen;
t_midiOutPrepareHeader o_midiOutPrepareHeader;
t_midiOutReset o_midiOutReset;
t_midiOutSetVolume o_midiOutSetVolume;
t_midiOutShortMsg o_midiOutShortMsg;
t_midiOutUnprepareHeader o_midiOutUnprepareHeader;
t_midiStreamClose o_midiStreamClose;
t_midiStreamOpen o_midiStreamOpen;
t_midiStreamOut o_midiStreamOut;
t_midiStreamPause o_midiStreamPause;
t_midiStreamPosition o_midiStreamPosition;
t_midiStreamProperty o_midiStreamProperty;
t_midiStreamRestart o_midiStreamRestart;
t_midiStreamStop o_midiStreamStop;
t_mixerClose o_mixerClose;
t_mixerGetControlDetailsA o_mixerGetControlDetailsA;
t_mixerGetControlDetailsW o_mixerGetControlDetailsW;
t_mixerGetDevCapsA o_mixerGetDevCapsA;
t_mixerGetDevCapsW o_mixerGetDevCapsW;
t_mixerGetID o_mixerGetID;
t_mixerGetLineControlsA o_mixerGetLineControlsA;
t_mixerGetLineControlsW o_mixerGetLineControlsW;
t_mixerGetLineInfoA o_mixerGetLineInfoA;
t_mixerGetLineInfoW o_mixerGetLineInfoW;
t_mixerGetNumDevs o_mixerGetNumDevs;
t_mixerMessage o_mixerMessage;
t_mixerOpen o_mixerOpen;
t_mixerSetControlDetails o_mixerSetControlDetails;
t_mmDrvInstall o_mmDrvInstall;
t_mmGetCurrentTask o_mmGetCurrentTask;
t_mmTaskBlock o_mmTaskBlock;
t_mmTaskCreate o_mmTaskCreate;
t_mmTaskSignal o_mmTaskSignal;
t_mmTaskYield o_mmTaskYield;
t_mmioAdvance o_mmioAdvance;
t_mmioAscend o_mmioAscend;
t_mmioClose o_mmioClose;
t_mmioCreateChunk o_mmioCreateChunk;
t_mmioDescend o_mmioDescend;
t_mmioFlush o_mmioFlush;
t_mmioGetInfo o_mmioGetInfo;
t_mmioInstallIOProcA o_mmioInstallIOProcA;
t_mmioInstallIOProcW o_mmioInstallIOProcW;
t_mmioOpenA o_mmioOpenA;
t_mmioOpenW o_mmioOpenW;
t_mmioRead o_mmioRead;
t_mmioRenameA o_mmioRenameA;
t_mmioRenameW o_mmioRenameW;
t_mmioSeek o_mmioSeek;
t_mmioSendMessage o_mmioSendMessage;
t_mmioSetBuffer o_mmioSetBuffer;
t_mmioSetInfo o_mmioSetInfo;
t_mmioStringToFOURCCA o_mmioStringToFOURCCA;
t_mmioStringToFOURCCW o_mmioStringToFOURCCW;
t_mmioWrite o_mmioWrite;
t_mmsystemGetVersion o_mmsystemGetVersion;
t_sndPlaySoundA o_sndPlaySoundA;
t_sndPlaySoundW o_sndPlaySoundW;
t_timeBeginPeriod o_timeBeginPeriod;
t_timeEndPeriod o_timeEndPeriod;
t_timeGetDevCaps o_timeGetDevCaps;
t_timeGetSystemTime o_timeGetSystemTime;
t_timeGetTime o_timeGetTime;
t_timeKillEvent o_timeKillEvent;
t_timeSetEvent o_timeSetEvent;
t_waveInAddBuffer o_waveInAddBuffer;
t_waveInClose o_waveInClose;
t_waveInGetDevCapsA o_waveInGetDevCapsA;
t_waveInGetDevCapsW o_waveInGetDevCapsW;
t_waveInGetErrorTextA o_waveInGetErrorTextA;
t_waveInGetErrorTextW o_waveInGetErrorTextW;
t_waveInGetID o_waveInGetID;
t_waveInGetNumDevs o_waveInGetNumDevs;
t_waveInGetPosition o_waveInGetPosition;
t_waveInMessage o_waveInMessage;
t_waveInOpen o_waveInOpen;
t_waveInPrepareHeader o_waveInPrepareHeader;
t_waveInReset o_waveInReset;
t_waveInStart o_waveInStart;
t_waveInStop o_waveInStop;
t_waveInUnprepareHeader o_waveInUnprepareHeader;
t_waveOutBreakLoop o_waveOutBreakLoop;
t_waveOutClose o_waveOutClose;
t_waveOutGetDevCapsA o_waveOutGetDevCapsA;
t_waveOutGetDevCapsW o_waveOutGetDevCapsW;
t_waveOutGetErrorTextA o_waveOutGetErrorTextA;
t_waveOutGetErrorTextW o_waveOutGetErrorTextW;
t_waveOutGetID o_waveOutGetID;
t_waveOutGetNumDevs o_waveOutGetNumDevs;
t_waveOutGetPitch o_waveOutGetPitch;
t_waveOutGetPlaybackRate o_waveOutGetPlaybackRate;
t_waveOutGetPosition o_waveOutGetPosition;
t_waveOutGetVolume o_waveOutGetVolume;
t_waveOutMessage o_waveOutMessage;
t_waveOutOpen o_waveOutOpen;
t_waveOutPause o_waveOutPause;
t_waveOutPrepareHeader o_waveOutPrepareHeader;
t_waveOutReset o_waveOutReset;
t_waveOutRestart o_waveOutRestart;
t_waveOutSetPitch o_waveOutSetPitch;
t_waveOutSetPlaybackRate o_waveOutSetPlaybackRate;
t_waveOutSetVolume o_waveOutSetVolume;
t_waveOutUnprepareHeader o_waveOutUnprepareHeader;
t_waveOutWrite o_waveOutWrite;

bool WINAPI hkmciExecute(LPCSTR pszCommand)
{
	return o_mciExecute(pszCommand);
}

LRESULT WINAPI hkCloseDriver(HDRVR hDriver, LPARAM lParam1, LPARAM lParam2)
{
	return o_CloseDriver(hDriver, lParam1, lParam2);
}

LRESULT WINAPI hkDefDriverProc(DWORD_PTR dwDriverIdentifier, HDRVR hdrvr, UINT uMsg, LPARAM lParam1, LPARAM lParam2)
{
	return o_DefDriverProc(dwDriverIdentifier, hdrvr, uMsg, lParam1, lParam2);
}

bool APIENTRY hkDriverCallback(DWORD_PTR dwCallback, DWORD dwFlags, HDRVR hDevice, DWORD dwMsg, DWORD_PTR dwUser, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	return o_DriverCallback(dwCallback, dwFlags, hDevice, dwMsg, dwUser, dwParam1, dwParam2);
}

HMODULE WINAPI hkDrvGetModuleHandle(HDRVR hDriver)
{
	return o_DrvGetModuleHandle(hDriver);
}

HMODULE WINAPI hkGetDriverModuleHandle(HDRVR hDriver)
{
	return o_GetDriverModuleHandle(hDriver);
}

HDRVR WINAPI hkOpenDriver(LPCWSTR szDriverName, LPCWSTR szSectionName, LPARAM lParam2)
{
	return o_OpenDriver(szDriverName, szSectionName, lParam2);
}

bool WINAPI hkPlaySound(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
	return o_PlaySound(pszSound, hmod, fdwSound);
}

bool WINAPI hkPlaySoundA(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
	return o_PlaySoundA(pszSound, hmod, fdwSound);
}

bool WINAPI hkPlaySoundW(LPCWSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
	return o_PlaySoundW(pszSound, hmod, fdwSound);
}

LRESULT WINAPI hkSendDriverMessage(HDRVR hDriver, UINT message, LPARAM lParam1, LPARAM lParam2)
{
	return o_SendDriverMessage(hDriver, message, lParam1, lParam2);
}

void WINAPI hkWOWAppExit()
{
	return o_WOWAppExit();
}

MMRESULT WINAPI hkauxGetDevCapsA(UINT_PTR uDeviceID, LPAUXCAPSA pac, UINT cbac)
{
	return o_auxGetDevCapsA(uDeviceID, pac, cbac);
}

MMRESULT WINAPI hkauxGetDevCapsW(UINT_PTR uDeviceID, LPAUXCAPSW pac, UINT cbac)
{
	return o_auxGetDevCapsW(uDeviceID, pac, cbac);
}

UINT WINAPI hkauxGetNumDevs()
{
	return o_auxGetNumDevs();
}

MMRESULT WINAPI hkauxGetVolume(UINT uDeviceID, LPDWORD pdwVolume)
{
	return o_auxGetVolume(uDeviceID, pdwVolume);
}

MMRESULT WINAPI hkauxOutMessage(UINT uDeviceID, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
	return o_auxOutMessage(uDeviceID, uMsg, dw1, dw2);
}

MMRESULT WINAPI hkauxSetVolume(UINT uDeviceID, DWORD dwVolume)
{
	return o_auxSetVolume(uDeviceID, dwVolume);
}

MMRESULT WINAPI hkjoyConfigChanged(DWORD dwFlags)
{
	return o_joyConfigChanged(dwFlags);
}

MMRESULT WINAPI hkjoyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc)
{
	return o_joyGetDevCapsA(uJoyID, pjc, cbjc);
}

MMRESULT WINAPI hkjoyGetDevCapsW(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc)
{
	return o_joyGetDevCapsW(uJoyID, pjc, cbjc);
}

UINT WINAPI hkjoyGetNumDevs()
{
	return o_joyGetNumDevs();
}

MMRESULT WINAPI hkjoyGetPos(UINT uJoyID, LPJOYINFO pji)
{
	return o_joyGetPos(uJoyID, pji);
}

MMRESULT WINAPI hkjoyGetPosEx(UINT uJoyID, LPJOYINFOEX pji)
{
	return o_joyGetPosEx(uJoyID, pji);
}

MMRESULT WINAPI hkjoyGetThreshold(UINT uJoyID, LPUINT puThreshold)
{
	return o_joyGetThreshold(uJoyID, puThreshold);
}

MMRESULT WINAPI hkjoyReleaseCapture(UINT uJoyID)
{
	return o_joyReleaseCapture(uJoyID);
}

MMRESULT WINAPI hkjoySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, bool fChanged)
{
	return o_joySetCapture(hwnd, uJoyID, uPeriod, fChanged);
}

MMRESULT WINAPI hkjoySetThreshold(UINT uJoyID, UINT uThreshold)
{
	return o_joySetThreshold(uJoyID, uThreshold);
}

bool APIENTRY hkmciDriverNotify(HANDLE hwndCallback, MCIDEVICEID wDeviceID, UINT uStatus)
{
	return o_mciDriverNotify(hwndCallback, wDeviceID, uStatus);
}

UINT APIENTRY hkmciDriverYield(MCIDEVICEID wDeviceID)
{
	return o_mciDriverYield(wDeviceID);
}

bool APIENTRY hkmciFreeCommandResource(UINT wTable)
{
	return o_mciFreeCommandResource(wTable);
}

HTASK WINAPI hkmciGetCreatorTask(MCIDEVICEID mciId)
{
	return o_mciGetCreatorTask(mciId);
}

MCIDEVICEID WINAPI hkmciGetDeviceIDA(LPCSTR pszDevice)
{
	return o_mciGetDeviceIDA(pszDevice);
}

MCIDEVICEID WINAPI hkmciGetDeviceIDFromElementIDA(DWORD dwElementID, LPCSTR lpstrType)
{
	return o_mciGetDeviceIDFromElementIDA(dwElementID, lpstrType);
}

MCIDEVICEID WINAPI hkmciGetDeviceIDFromElementIDW(DWORD dwElementID, LPCWSTR lpstrType)
{
	return o_mciGetDeviceIDFromElementIDW(dwElementID, lpstrType);
}

MCIDEVICEID WINAPI hkmciGetDeviceIDW(LPCWSTR pszDevice)
{
	return o_mciGetDeviceIDW(pszDevice);
}

DWORD_PTR APIENTRY hkmciGetDriverData(MCIDEVICEID wDeviceID)
{
	return o_mciGetDriverData(wDeviceID);
}

bool WINAPI hkmciGetErrorStringA(MCIERROR mcierr, LPSTR pszText, UINT cchText)
{
	return o_mciGetErrorStringA(mcierr, pszText, cchText);
}

bool WINAPI hkmciGetErrorStringW(MCIERROR mcierr, LPWSTR pszText, UINT cchText)
{
	return o_mciGetErrorStringW(mcierr, pszText, cchText);
}

YIELDPROC WINAPI hkmciGetYieldProc(MCIDEVICEID mciId, LPDWORD pdwYieldData)
{
	return o_mciGetYieldProc(mciId, pdwYieldData);
}

UINT APIENTRY hkmciLoadCommandResource(HANDLE hInstance, LPCWSTR lpResName, UINT wType)
{
	return o_mciLoadCommandResource(hInstance, lpResName, wType);
}

MCIERROR WINAPI hkmciSendCommandA(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	return o_mciSendCommandA(mciId, uMsg, dwParam1, dwParam2);
}

MCIERROR WINAPI hkmciSendCommandW(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	return o_mciSendCommandW(mciId, uMsg, dwParam1, dwParam2);
}

MCIERROR WINAPI hkmciSendStringA(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback)
{
	return o_mciSendStringA(lpstrCommand, lpstrReturnString, uReturnLength, hwndCallback);
}

MCIERROR WINAPI hkmciSendStringW(LPCWSTR lpstrCommand, LPWSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback)
{
	return o_mciSendStringW(lpstrCommand, lpstrReturnString, uReturnLength, hwndCallback);
}

bool APIENTRY hkmciSetDriverData(MCIDEVICEID wDeviceID, DWORD_PTR dwData)
{
	return o_mciSetDriverData(wDeviceID, dwData);
}

bool WINAPI hkmciSetYieldProc(MCIDEVICEID mciId, YIELDPROC fpYieldProc, DWORD dwYieldData)
{
	return o_mciSetYieldProc(mciId, fpYieldProc, dwYieldData);
}

MMRESULT WINAPI hkmidiConnect(HMIDI hmi, HMIDIOUT hmo, LPVOID pReserved)
{
	return o_midiConnect(hmi, hmo, pReserved);
}

MMRESULT WINAPI hkmidiDisconnect(HMIDI hmi, HMIDIOUT hmo, LPVOID pReserved)
{
	return o_midiDisconnect(hmi, hmo, pReserved);
}

MMRESULT WINAPI hkmidiInAddBuffer(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh)
{
	return o_midiInAddBuffer(hmi, pmh, cbmh);
}

MMRESULT WINAPI hkmidiInClose(HMIDIIN hmi)
{
	return o_midiInClose(hmi);
}

MMRESULT WINAPI hkmidiInGetDevCapsA(UINT_PTR uDeviceID, LPMIDIINCAPSA pmic, UINT cbmic)
{
	return o_midiInGetDevCapsA(uDeviceID, pmic, cbmic);
}

MMRESULT WINAPI hkmidiInGetDevCapsW(UINT_PTR uDeviceID, LPMIDIINCAPSW pmic, UINT cbmic)
{
	return o_midiInGetDevCapsW(uDeviceID, pmic, cbmic);
}

MMRESULT WINAPI hkmidiInGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText)
{
	return o_midiInGetErrorTextA(mmrError, pszText, cchText);
}

MMRESULT WINAPI hkmidiInGetErrorTextW(MMRESULT mmrError, LPWSTR pszText, UINT cchText)
{
	return o_midiInGetErrorTextW(mmrError, pszText, cchText);
}

MMRESULT WINAPI hkmidiInGetID(HMIDIIN hmi, LPUINT puDeviceID)
{
	return o_midiInGetID(hmi, puDeviceID);
}

UINT WINAPI hkmidiInGetNumDevs()
{
	return o_midiInGetNumDevs();
}

MMRESULT WINAPI hkmidiInMessage(HMIDIIN hmi, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
	return o_midiInMessage(hmi, uMsg, dw1, dw2);
}

MMRESULT WINAPI hkmidiInOpen(LPHMIDIIN phmi, UINT uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
	return o_midiInOpen(phmi, uDeviceID, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI hkmidiInPrepareHeader(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh)
{
	return o_midiInPrepareHeader(hmi, pmh, cbmh);
}

MMRESULT WINAPI hkmidiInReset(HMIDIIN hmi)
{
	return o_midiInReset(hmi);
}

MMRESULT WINAPI hkmidiInStart(HMIDIIN hmi)
{
	return o_midiInStart(hmi);
}

MMRESULT WINAPI hkmidiInStop(HMIDIIN hmi)
{
	return o_midiInStop(hmi);
}

MMRESULT WINAPI hkmidiInUnprepareHeader(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh)
{
	return o_midiInUnprepareHeader(hmi, pmh, cbmh);
}

MMRESULT WINAPI hkmidiOutCacheDrumPatches(HMIDIOUT hmo, UINT uPatch, LPWORD pwkya, UINT fuCache)
{
	return o_midiOutCacheDrumPatches(hmo, uPatch, pwkya, fuCache);
}

MMRESULT WINAPI hkmidiOutCachePatches(HMIDIOUT hmo, UINT uBank, LPWORD pwpa, UINT fuCache)
{
	return o_midiOutCachePatches(hmo, uBank, pwpa, fuCache);
}

MMRESULT WINAPI hkmidiOutClose(HMIDIOUT hmo)
{
	return o_midiOutClose(hmo);
}

MMRESULT WINAPI hkmidiOutGetDevCapsA(UINT_PTR uDeviceID, LPMIDIOUTCAPSA pmoc, UINT cbmoc)
{
	return o_midiOutGetDevCapsA(uDeviceID, pmoc, cbmoc);
}

MMRESULT WINAPI hkmidiOutGetDevCapsW(UINT_PTR uDeviceID, LPMIDIOUTCAPSW pmoc, UINT cbmoc)
{
	return o_midiOutGetDevCapsW(uDeviceID, pmoc, cbmoc);
}

MMRESULT WINAPI hkmidiOutGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText)
{
	return o_midiOutGetErrorTextA(mmrError, pszText, cchText);
}

MMRESULT WINAPI hkmidiOutGetErrorTextW(MMRESULT mmrError, LPWSTR pszText, UINT cchText)
{
	return o_midiOutGetErrorTextW(mmrError, pszText, cchText);
}

MMRESULT WINAPI hkmidiOutGetID(HMIDIOUT hmo, LPUINT puDeviceID)
{
	return o_midiOutGetID(hmo, puDeviceID);
}

UINT WINAPI hkmidiOutGetNumDevs()
{
	return o_midiOutGetNumDevs();
}

MMRESULT WINAPI hkmidiOutGetVolume(HMIDIOUT hmo, LPDWORD pdwVolume)
{
	return o_midiOutGetVolume(hmo, pdwVolume);
}

MMRESULT WINAPI hkmidiOutLongMsg(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh)
{
	return o_midiOutLongMsg(hmo, pmh, cbmh);
}

MMRESULT WINAPI hkmidiOutMessage(HMIDIOUT hmo, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
	return o_midiOutMessage(hmo, uMsg, dw1, dw2);
}

MMRESULT WINAPI hkmidiOutOpen(LPHMIDIOUT phmo, UINT uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
	return o_midiOutOpen(phmo, uDeviceID, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI hkmidiOutPrepareHeader(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh)
{
	return o_midiOutPrepareHeader(hmo, pmh, cbmh);
}

MMRESULT WINAPI hkmidiOutReset(HMIDIOUT hmo)
{
	return o_midiOutReset(hmo);
}

MMRESULT WINAPI hkmidiOutSetVolume(HMIDIOUT hmo, DWORD dwVolume)
{
	return o_midiOutSetVolume(hmo, dwVolume);
}

MMRESULT WINAPI hkmidiOutShortMsg(HMIDIOUT hmo, DWORD dwMsg)
{
	return o_midiOutShortMsg(hmo, dwMsg);
}

MMRESULT WINAPI hkmidiOutUnprepareHeader(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh)
{
	return o_midiOutUnprepareHeader(hmo, pmh, cbmh);
}

MMRESULT WINAPI hkmidiStreamClose(HMIDISTRM hms)
{
	return o_midiStreamClose(hms);
}

MMRESULT WINAPI hkmidiStreamOpen(LPHMIDISTRM phms, LPUINT puDeviceID, DWORD cMidi, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
	return o_midiStreamOpen(phms, puDeviceID, cMidi, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI hkmidiStreamOut(HMIDISTRM hms, LPMIDIHDR pmh, UINT cbmh)
{
	return o_midiStreamOut(hms, pmh, cbmh);
}

MMRESULT WINAPI hkmidiStreamPause(HMIDISTRM hms)
{
	return o_midiStreamPause(hms);
}

MMRESULT WINAPI hkmidiStreamPosition(HMIDISTRM hms, LPMMTIME lpmmt, UINT cbmmt)
{
	return o_midiStreamPosition(hms, lpmmt, cbmmt);
}

MMRESULT WINAPI hkmidiStreamProperty(HMIDISTRM hms, LPBYTE lppropdata, DWORD dwProperty)
{
	return o_midiStreamProperty(hms, lppropdata, dwProperty);
}

MMRESULT WINAPI hkmidiStreamRestart(HMIDISTRM hms)
{
	return o_midiStreamRestart(hms);
}

MMRESULT WINAPI hkmidiStreamStop(HMIDISTRM hms)
{
	return o_midiStreamStop(hms);
}

MMRESULT WINAPI hkmixerClose(HMIXER hmx)
{
	return o_mixerClose(hmx);
}

MMRESULT WINAPI hkmixerGetControlDetailsA(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails)
{
	return o_mixerGetControlDetailsA(hmxobj, pmxcd, fdwDetails);
}

MMRESULT WINAPI hkmixerGetControlDetailsW(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails)
{
	return o_mixerGetControlDetailsW(hmxobj, pmxcd, fdwDetails);
}

MMRESULT WINAPI hkmixerGetDevCapsA(UINT_PTR uMxId, LPMIXERCAPSA pmxcaps, UINT cbmxcaps)
{
	return o_mixerGetDevCapsA(uMxId, pmxcaps, cbmxcaps);
}

MMRESULT WINAPI hkmixerGetDevCapsW(UINT_PTR uMxId, LPMIXERCAPSW pmxcaps, UINT cbmxcaps)
{
	return o_mixerGetDevCapsW(uMxId, pmxcaps, cbmxcaps);
}

MMRESULT WINAPI hkmixerGetID(HMIXEROBJ hmxobj, UINT FAR* puMxId, DWORD fdwId)
{
	return o_mixerGetID(hmxobj, puMxId, fdwId);
}

MMRESULT WINAPI hkmixerGetLineControlsA(HMIXEROBJ hmxobj, LPMIXERLINECONTROLSA pmxlc, DWORD fdwControls)
{
	return o_mixerGetLineControlsA(hmxobj, pmxlc, fdwControls);
}

MMRESULT WINAPI hkmixerGetLineControlsW(HMIXEROBJ hmxobj, LPMIXERLINECONTROLSW pmxlc, DWORD fdwControls)
{
	return o_mixerGetLineControlsW(hmxobj, pmxlc, fdwControls);
}

MMRESULT WINAPI hkmixerGetLineInfoA(HMIXEROBJ hmxobj, LPMIXERLINEA pmxl, DWORD fdwInfo)
{
	return o_mixerGetLineInfoA(hmxobj, pmxl, fdwInfo);
}

MMRESULT WINAPI hkmixerGetLineInfoW(HMIXEROBJ hmxobj, LPMIXERLINEW pmxl, DWORD fdwInfo)
{
	return o_mixerGetLineInfoW(hmxobj, pmxl, fdwInfo);
}

UINT WINAPI hkmixerGetNumDevs()
{
	return o_mixerGetNumDevs();
}

DWORD WINAPI hkmixerMessage(HMIXER hmx, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	return o_mixerMessage(hmx, uMsg, dwParam1, dwParam2);
}

MMRESULT WINAPI hkmixerOpen(LPHMIXER phmx, UINT uMxId, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
	return o_mixerOpen(phmx, uMxId, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI hkmixerSetControlDetails(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails)
{
	return o_mixerSetControlDetails(hmxobj, pmxcd, fdwDetails);
}

UINT APIENTRY hkmmDrvInstall(HDRVR hDriver, LPCWSTR wszDrvEntry, DRIVERMSGPROC drvMessage, UINT wFlags)
{
	return o_mmDrvInstall(hDriver, wszDrvEntry, drvMessage, wFlags);
}

DWORD APIENTRY hkmmGetCurrentTask()
{
	return o_mmGetCurrentTask();
}

VOID APIENTRY hkmmTaskBlock(DWORD h)
{
	return o_mmTaskBlock(h);
}

UINT APIENTRY hkmmTaskCreate(LPTASKCALLBACK lpfn, HANDLE FAR* lph, DWORD_PTR dwInst)
{
	return o_mmTaskCreate(lpfn, lph, dwInst);
}

bool APIENTRY hkmmTaskSignal(DWORD h)
{
	return o_mmTaskSignal(h);
}

VOID APIENTRY hkmmTaskYield()
{
	return o_mmTaskYield();
}

MMRESULT WINAPI hkmmioAdvance(HMMIO hmmio, LPMMIOINFO pmmioinfo, UINT fuAdvance)
{
	return o_mmioAdvance(hmmio, pmmioinfo, fuAdvance);
}

MMRESULT WINAPI hkmmioAscend(HMMIO hmmio, LPMMCKINFO pmmcki, UINT fuAscend)
{
	return o_mmioAscend(hmmio, pmmcki, fuAscend);
}

MMRESULT WINAPI hkmmioClose(HMMIO hmmio, UINT fuClose)
{
	return o_mmioClose(hmmio, fuClose);
}

MMRESULT WINAPI hkmmioCreateChunk(HMMIO hmmio, LPMMCKINFO pmmcki, UINT fuCreate)
{
	return o_mmioCreateChunk(hmmio, pmmcki, fuCreate);
}

MMRESULT WINAPI hkmmioDescend(HMMIO hmmio, LPMMCKINFO pmmcki, const MMCKINFO FAR* pmmckiParent, UINT fuDescend)
{
	return o_mmioDescend(hmmio, pmmcki, pmmckiParent, fuDescend);
}

MMRESULT WINAPI hkmmioFlush(HMMIO hmmio, UINT fuFlush)
{
	return o_mmioFlush(hmmio, fuFlush);
}

MMRESULT WINAPI hkmmioGetInfo(HMMIO hmmio, LPMMIOINFO pmmioinfo, UINT fuInfo)
{
	return o_mmioGetInfo(hmmio, pmmioinfo, fuInfo);
}

LPMMIOPROC WINAPI hkmmioInstallIOProcA(FOURCC fccIOProc, LPMMIOPROC pIOProc, DWORD dwFlags)
{
	return o_mmioInstallIOProcA(fccIOProc, pIOProc, dwFlags);
}

LPMMIOPROC WINAPI hkmmioInstallIOProcW(FOURCC fccIOProc, LPMMIOPROC pIOProc, DWORD dwFlags)
{
	return o_mmioInstallIOProcW(fccIOProc, pIOProc, dwFlags);
}

HMMIO WINAPI hkmmioOpenA(LPSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen)
{
	return o_mmioOpenA(pszFileName, pmmioinfo, fdwOpen);
}

HMMIO WINAPI hkmmioOpenW(LPWSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen)
{
	return o_mmioOpenW(pszFileName, pmmioinfo, fdwOpen);
}

LONG WINAPI hkmmioRead(HMMIO hmmio, HPSTR pch, LONG cch)
{
	return o_mmioRead(hmmio, pch, cch);
}

MMRESULT WINAPI hkmmioRenameA(LPCSTR pszFileName, LPCSTR pszNewFileName, LPCMMIOINFO pmmioinfo, DWORD fdwRename)
{
	return o_mmioRenameA(pszFileName, pszNewFileName, pmmioinfo, fdwRename);
}

MMRESULT WINAPI hkmmioRenameW(LPCWSTR pszFileName, LPCWSTR pszNewFileName, LPCMMIOINFO pmmioinfo, DWORD fdwRename)
{
	return o_mmioRenameW(pszFileName, pszNewFileName, pmmioinfo, fdwRename);
}

LONG WINAPI hkmmioSeek(HMMIO hmmio, LONG lOffset, int iOrigin)
{
	return o_mmioSeek(hmmio, lOffset, iOrigin);
}

LRESULT WINAPI hkmmioSendMessage(HMMIO hmmio, UINT uMsg, LPARAM lParam1, LPARAM lParam2)
{
	return o_mmioSendMessage(hmmio, uMsg, lParam1, lParam2);
}

MMRESULT WINAPI hkmmioSetBuffer(HMMIO hmmio, LPSTR pchBuffer, LONG cchBuffer, UINT fuBuffer)
{
	return o_mmioSetBuffer(hmmio, pchBuffer, cchBuffer, fuBuffer);
}

MMRESULT WINAPI hkmmioSetInfo(HMMIO hmmio, LPCMMIOINFO pmmioinfo, UINT fuInfo)
{
	return o_mmioSetInfo(hmmio, pmmioinfo, fuInfo);
}

FOURCC WINAPI hkmmioStringToFOURCCA(LPCSTR sz, UINT uFlags)
{
	return o_mmioStringToFOURCCA(sz, uFlags);
}

FOURCC WINAPI hkmmioStringToFOURCCW(LPCWSTR sz, UINT uFlags)
{
	return o_mmioStringToFOURCCW(sz, uFlags);
}

LONG WINAPI hkmmioWrite(HMMIO hmmio, const char _huge* pch, LONG cch)
{
	return o_mmioWrite(hmmio, pch, cch);
}

UINT WINAPI hkmmsystemGetVersion()
{
	return o_mmsystemGetVersion();
}

bool WINAPI hksndPlaySoundA(LPCSTR pszSound, UINT fuSound)
{
	return o_sndPlaySoundA(pszSound, fuSound);
}

bool WINAPI hksndPlaySoundW(LPCWSTR pszSound, UINT fuSound)
{
	return o_sndPlaySoundW(pszSound, fuSound);
}

MMRESULT WINAPI hktimeBeginPeriod(UINT uPeriod)
{
	return o_timeBeginPeriod(uPeriod);
}

MMRESULT WINAPI hktimeEndPeriod(UINT uPeriod)
{
	return o_timeEndPeriod(uPeriod);
}

MMRESULT WINAPI hktimeGetDevCaps(LPTIMECAPS ptc, UINT cbtc)
{
	return o_timeGetDevCaps(ptc, cbtc);
}

MMRESULT WINAPI hktimeGetSystemTime(LPMMTIME pmmt, UINT cbmmt)
{
	return o_timeGetSystemTime(pmmt, cbmmt);
}

DWORD WINAPI hktimeGetTime()
{
	return o_timeGetTime();
}

MMRESULT WINAPI hktimeKillEvent(UINT uTimerID)
{
	return o_timeKillEvent(uTimerID);
}

MMRESULT WINAPI hktimeSetEvent(UINT uDelay, UINT uResolution, LPTIMECALLBACK fptc, DWORD_PTR dwUser, UINT fuEvent)
{
	return o_timeSetEvent(uDelay, uResolution, fptc, dwUser, fuEvent);
}

MMRESULT WINAPI hkwaveInAddBuffer(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh)
{
	return o_waveInAddBuffer(hwi, pwh, cbwh);
}

MMRESULT WINAPI hkwaveInClose(HWAVEIN hwi)
{
	return o_waveInClose(hwi);
}

MMRESULT WINAPI hkwaveInGetDevCapsA(UINT_PTR uDeviceID, LPWAVEINCAPSA pwic, UINT cbwic)
{
	return o_waveInGetDevCapsA(uDeviceID, pwic, cbwic);
}

MMRESULT WINAPI hkwaveInGetDevCapsW(UINT_PTR uDeviceID, LPWAVEINCAPSW pwic, UINT cbwic)
{
	return o_waveInGetDevCapsW(uDeviceID, pwic, cbwic);
}

MMRESULT WINAPI hkwaveInGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText)
{
	return o_waveInGetErrorTextA(mmrError, pszText, cchText);
}

MMRESULT WINAPI hkwaveInGetErrorTextW(MMRESULT mmrError, LPWSTR pszText, UINT cchText)
{
	return o_waveInGetErrorTextW(mmrError, pszText, cchText);
}

MMRESULT WINAPI hkwaveInGetID(HWAVEIN hwi, LPUINT puDeviceID)
{
	return o_waveInGetID(hwi, puDeviceID);
}

UINT WINAPI hkwaveInGetNumDevs()
{
	return o_waveInGetNumDevs();
}

MMRESULT WINAPI hkwaveInGetPosition(HWAVEIN hwi, LPMMTIME pmmt, UINT cbmmt)
{
	return o_waveInGetPosition(hwi, pmmt, cbmmt);
}

MMRESULT WINAPI hkwaveInMessage(HWAVEIN hwi, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
	return o_waveInMessage(hwi, uMsg, dw1, dw2);
}

MMRESULT WINAPI hkwaveInOpen(LPHWAVEIN phwi, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
	return o_waveInOpen(phwi, uDeviceID, pwfx, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI hkwaveInPrepareHeader(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh)
{
	return o_waveInPrepareHeader(hwi, pwh, cbwh);
}

MMRESULT WINAPI hkwaveInReset(HWAVEIN hwi)
{
	return o_waveInReset(hwi);
}

MMRESULT WINAPI hkwaveInStart(HWAVEIN hwi)
{
	return o_waveInStart(hwi);
}

MMRESULT WINAPI hkwaveInStop(HWAVEIN hwi)
{
	return o_waveInStop(hwi);
}

MMRESULT WINAPI hkwaveInUnprepareHeader(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh)
{
	return o_waveInUnprepareHeader(hwi, pwh, cbwh);
}

MMRESULT WINAPI hkwaveOutBreakLoop(HWAVEOUT hwo)
{
	return o_waveOutBreakLoop(hwo);
}

MMRESULT WINAPI hkwaveOutClose(HWAVEOUT hwo)
{
	return o_waveOutClose(hwo);
}

MMRESULT WINAPI hkwaveOutGetDevCapsA(UINT_PTR uDeviceID, LPWAVEOUTCAPSA pwoc, UINT cbwoc)
{
	return o_waveOutGetDevCapsA(uDeviceID, pwoc, cbwoc);
}

MMRESULT WINAPI hkwaveOutGetDevCapsW(UINT_PTR uDeviceID, LPWAVEOUTCAPSW pwoc, UINT cbwoc)
{
	return o_waveOutGetDevCapsW(uDeviceID, pwoc, cbwoc);
}

MMRESULT WINAPI hkwaveOutGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText)
{
	return o_waveOutGetErrorTextA(mmrError, pszText, cchText);
}

MMRESULT WINAPI hkwaveOutGetErrorTextW(MMRESULT mmrError, LPWSTR pszText, UINT cchText)
{
	return o_waveOutGetErrorTextW(mmrError, pszText, cchText);
}

MMRESULT WINAPI hkwaveOutGetID(HWAVEOUT hwo, LPUINT puDeviceID)
{
	return o_waveOutGetID(hwo, puDeviceID);
}

UINT WINAPI hkwaveOutGetNumDevs()
{
	return o_waveOutGetNumDevs();
}

MMRESULT WINAPI hkwaveOutGetPitch(HWAVEOUT hwo, LPDWORD pdwPitch)
{
	return o_waveOutGetPitch(hwo, pdwPitch);
}

MMRESULT WINAPI hkwaveOutGetPlaybackRate(HWAVEOUT hwo, LPDWORD pdwRate)
{
	return o_waveOutGetPlaybackRate(hwo, pdwRate);
}

MMRESULT WINAPI hkwaveOutGetPosition(HWAVEOUT hwo, LPMMTIME pmmt, UINT cbmmt)
{
	return o_waveOutGetPosition(hwo, pmmt, cbmmt);
}

MMRESULT WINAPI hkwaveOutGetVolume(HWAVEOUT hwo, LPDWORD pdwVolume)
{
	return o_waveOutGetVolume(hwo, pdwVolume);
}

MMRESULT WINAPI hkwaveOutMessage(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
	return o_waveOutMessage(hwo, uMsg, dw1, dw2);
}

MMRESULT WINAPI hkwaveOutOpen(LPHWAVEOUT phwo, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
	return o_waveOutOpen(phwo, uDeviceID, pwfx, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI hkwaveOutPause(HWAVEOUT hwo)
{
	return o_waveOutPause(hwo);
}

MMRESULT WINAPI hkwaveOutPrepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
	return o_waveOutPrepareHeader(hwo, pwh, cbwh);
}

MMRESULT WINAPI hkwaveOutReset(HWAVEOUT hwo)
{
	return o_waveOutReset(hwo);
}

MMRESULT WINAPI hkwaveOutRestart(HWAVEOUT hwo)
{
	return o_waveOutRestart(hwo);
}

MMRESULT WINAPI hkwaveOutSetPitch(HWAVEOUT hwo, DWORD dwPitch)
{
	return o_waveOutSetPitch(hwo, dwPitch);
}

MMRESULT WINAPI hkwaveOutSetPlaybackRate(HWAVEOUT hwo, DWORD dwRate)
{
	return o_waveOutSetPlaybackRate(hwo, dwRate);
}

MMRESULT WINAPI hkwaveOutSetVolume(HWAVEOUT hwo, DWORD dwVolume)
{
	return o_waveOutSetVolume(hwo, dwVolume);
}

MMRESULT WINAPI hkwaveOutUnprepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
	return o_waveOutUnprepareHeader(hwo, pwh, cbwh);
}

MMRESULT WINAPI hkwaveOutWrite(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
	return o_waveOutWrite(hwo, pwh, cbwh);
}

void winmm(bool Free) {
	if (Free) {
		(void)FreeLibrary(winmm_dll);
		return;
	}

	char path[MAX_PATH]{};
	CopyMemory(path + GetSystemDirectory(path, MAX_PATH - 11), "\\winmm.dll", 11);
	winmm_dll = LoadLibrary(path);

	if (winmm_dll == NULL)
		MsgBoxExit(MB_ICONERROR, "Exiting", "Source winmm.dll missing");

	o_mciExecute = reinterpret_cast<t_mciExecute>(GetProcAddress(winmm_dll, "mciExecute"));
	o_CloseDriver = reinterpret_cast<t_CloseDriver>(GetProcAddress(winmm_dll, "CloseDriver"));
	o_DefDriverProc = reinterpret_cast<t_DefDriverProc>(GetProcAddress(winmm_dll, "DefDriverProc"));
	o_DriverCallback = reinterpret_cast<t_DriverCallback>(GetProcAddress(winmm_dll, "DriverCallback"));
	o_DrvGetModuleHandle = reinterpret_cast<t_DrvGetModuleHandle>(GetProcAddress(winmm_dll, "DrvGetModuleHandle"));
	o_GetDriverModuleHandle = reinterpret_cast<t_GetDriverModuleHandle>(GetProcAddress(winmm_dll, "GetDriverModuleHandle"));
	o_OpenDriver = reinterpret_cast<t_OpenDriver>(GetProcAddress(winmm_dll, "OpenDriver"));
	o_PlaySound = reinterpret_cast<t_PlaySound>(GetProcAddress(winmm_dll, "PlaySound"));
	o_PlaySoundA = reinterpret_cast<t_PlaySoundA>(GetProcAddress(winmm_dll, "PlaySoundA"));
	o_PlaySoundW = reinterpret_cast<t_PlaySoundW>(GetProcAddress(winmm_dll, "PlaySoundW"));
	o_SendDriverMessage = reinterpret_cast<t_SendDriverMessage>(GetProcAddress(winmm_dll, "SendDriverMessage"));
	o_WOWAppExit = reinterpret_cast<t_WOWAppExit>(GetProcAddress(winmm_dll, "WOWAppExit"));
	o_auxGetDevCapsA = reinterpret_cast<t_auxGetDevCapsA>(GetProcAddress(winmm_dll, "auxGetDevCapsA"));
	o_auxGetDevCapsW = reinterpret_cast<t_auxGetDevCapsW>(GetProcAddress(winmm_dll, "auxGetDevCapsW"));
	o_auxGetNumDevs = reinterpret_cast<t_auxGetNumDevs>(GetProcAddress(winmm_dll, "auxGetNumDevs"));
	o_auxGetVolume = reinterpret_cast<t_auxGetVolume>(GetProcAddress(winmm_dll, "auxGetVolume"));
	o_auxOutMessage = reinterpret_cast<t_auxOutMessage>(GetProcAddress(winmm_dll, "auxOutMessage"));
	o_auxSetVolume = reinterpret_cast<t_auxSetVolume>(GetProcAddress(winmm_dll, "auxSetVolume"));
	o_joyConfigChanged = reinterpret_cast<t_joyConfigChanged>(GetProcAddress(winmm_dll, "joyConfigChanged"));
	o_joyGetDevCapsA = reinterpret_cast<t_joyGetDevCapsA>(GetProcAddress(winmm_dll, "joyGetDevCapsA"));
	o_joyGetDevCapsW = reinterpret_cast<t_joyGetDevCapsW>(GetProcAddress(winmm_dll, "joyGetDevCapsW"));
	o_joyGetNumDevs = reinterpret_cast<t_joyGetNumDevs>(GetProcAddress(winmm_dll, "joyGetNumDevs"));
	o_joyGetPos = reinterpret_cast<t_joyGetPos>(GetProcAddress(winmm_dll, "joyGetPos"));
	o_joyGetPosEx = reinterpret_cast<t_joyGetPosEx>(GetProcAddress(winmm_dll, "joyGetPosEx"));
	o_joyGetThreshold = reinterpret_cast<t_joyGetThreshold>(GetProcAddress(winmm_dll, "joyGetThreshold"));
	o_joyReleaseCapture = reinterpret_cast<t_joyReleaseCapture>(GetProcAddress(winmm_dll, "joyReleaseCapture"));
	o_joySetCapture = reinterpret_cast<t_joySetCapture>(GetProcAddress(winmm_dll, "joySetCapture"));
	o_joySetThreshold = reinterpret_cast<t_joySetThreshold>(GetProcAddress(winmm_dll, "joySetThreshold"));
	o_mciDriverNotify = reinterpret_cast<t_mciDriverNotify>(GetProcAddress(winmm_dll, "mciDriverNotify"));
	o_mciDriverYield = reinterpret_cast<t_mciDriverYield>(GetProcAddress(winmm_dll, "mciDriverYield"));
	o_mciFreeCommandResource = reinterpret_cast<t_mciFreeCommandResource>(GetProcAddress(winmm_dll, "mciFreeCommandResource"));
	o_mciGetCreatorTask = reinterpret_cast<t_mciGetCreatorTask>(GetProcAddress(winmm_dll, "mciGetCreatorTask"));
	o_mciGetDeviceIDA = reinterpret_cast<t_mciGetDeviceIDA>(GetProcAddress(winmm_dll, "mciGetDeviceIDA"));
	o_mciGetDeviceIDFromElementIDA = reinterpret_cast<t_mciGetDeviceIDFromElementIDA>(GetProcAddress(winmm_dll, "mciGetDeviceIDFromElementIDA"));
	o_mciGetDeviceIDFromElementIDW = reinterpret_cast<t_mciGetDeviceIDFromElementIDW>(GetProcAddress(winmm_dll, "mciGetDeviceIDFromElementIDW"));
	o_mciGetDeviceIDW = reinterpret_cast<t_mciGetDeviceIDW>(GetProcAddress(winmm_dll, "mciGetDeviceIDW"));
	o_mciGetDriverData = reinterpret_cast<t_mciGetDriverData>(GetProcAddress(winmm_dll, "mciGetDriverData"));
	o_mciGetErrorStringA = reinterpret_cast<t_mciGetErrorStringA>(GetProcAddress(winmm_dll, "mciGetErrorStringA"));
	o_mciGetErrorStringW = reinterpret_cast<t_mciGetErrorStringW>(GetProcAddress(winmm_dll, "mciGetErrorStringW"));
	o_mciGetYieldProc = reinterpret_cast<t_mciGetYieldProc>(GetProcAddress(winmm_dll, "mciGetYieldProc"));
	o_mciLoadCommandResource = reinterpret_cast<t_mciLoadCommandResource>(GetProcAddress(winmm_dll, "mciLoadCommandResource"));
	o_mciSendCommandA = reinterpret_cast<t_mciSendCommandA>(GetProcAddress(winmm_dll, "mciSendCommandA"));
	o_mciSendCommandW = reinterpret_cast<t_mciSendCommandW>(GetProcAddress(winmm_dll, "mciSendCommandW"));
	o_mciSendStringA = reinterpret_cast<t_mciSendStringA>(GetProcAddress(winmm_dll, "mciSendStringA"));
	o_mciSendStringW = reinterpret_cast<t_mciSendStringW>(GetProcAddress(winmm_dll, "mciSendStringW"));
	o_mciSetDriverData = reinterpret_cast<t_mciSetDriverData>(GetProcAddress(winmm_dll, "mciSetDriverData"));
	o_mciSetYieldProc = reinterpret_cast<t_mciSetYieldProc>(GetProcAddress(winmm_dll, "mciSetYieldProc"));
	o_midiConnect = reinterpret_cast<t_midiConnect>(GetProcAddress(winmm_dll, "midiConnect"));
	o_midiDisconnect = reinterpret_cast<t_midiDisconnect>(GetProcAddress(winmm_dll, "midiDisconnect"));
	o_midiInAddBuffer = reinterpret_cast<t_midiInAddBuffer>(GetProcAddress(winmm_dll, "midiInAddBuffer"));
	o_midiInClose = reinterpret_cast<t_midiInClose>(GetProcAddress(winmm_dll, "midiInClose"));
	o_midiInGetDevCapsA = reinterpret_cast<t_midiInGetDevCapsA>(GetProcAddress(winmm_dll, "midiInGetDevCapsA"));
	o_midiInGetDevCapsW = reinterpret_cast<t_midiInGetDevCapsW>(GetProcAddress(winmm_dll, "midiInGetDevCapsW"));
	o_midiInGetErrorTextA = reinterpret_cast<t_midiInGetErrorTextA>(GetProcAddress(winmm_dll, "midiInGetErrorTextA"));
	o_midiInGetErrorTextW = reinterpret_cast<t_midiInGetErrorTextW>(GetProcAddress(winmm_dll, "midiInGetErrorTextW"));
	o_midiInGetID = reinterpret_cast<t_midiInGetID>(GetProcAddress(winmm_dll, "midiInGetID"));
	o_midiInGetNumDevs = reinterpret_cast<t_midiInGetNumDevs>(GetProcAddress(winmm_dll, "midiInGetNumDevs"));
	o_midiInMessage = reinterpret_cast<t_midiInMessage>(GetProcAddress(winmm_dll, "midiInMessage"));
	o_midiInOpen = reinterpret_cast<t_midiInOpen>(GetProcAddress(winmm_dll, "midiInOpen"));
	o_midiInPrepareHeader = reinterpret_cast<t_midiInPrepareHeader>(GetProcAddress(winmm_dll, "midiInPrepareHeader"));
	o_midiInReset = reinterpret_cast<t_midiInReset>(GetProcAddress(winmm_dll, "midiInReset"));
	o_midiInStart = reinterpret_cast<t_midiInStart>(GetProcAddress(winmm_dll, "midiInStart"));
	o_midiInStop = reinterpret_cast<t_midiInStop>(GetProcAddress(winmm_dll, "midiInStop"));
	o_midiInUnprepareHeader = reinterpret_cast<t_midiInUnprepareHeader>(GetProcAddress(winmm_dll, "midiInUnprepareHeader"));
	o_midiOutCacheDrumPatches = reinterpret_cast<t_midiOutCacheDrumPatches>(GetProcAddress(winmm_dll, "midiOutCacheDrumPatches"));
	o_midiOutCachePatches = reinterpret_cast<t_midiOutCachePatches>(GetProcAddress(winmm_dll, "midiOutCachePatches"));
	o_midiOutClose = reinterpret_cast<t_midiOutClose>(GetProcAddress(winmm_dll, "midiOutClose"));
	o_midiOutGetDevCapsA = reinterpret_cast<t_midiOutGetDevCapsA>(GetProcAddress(winmm_dll, "midiOutGetDevCapsA"));
	o_midiOutGetDevCapsW = reinterpret_cast<t_midiOutGetDevCapsW>(GetProcAddress(winmm_dll, "midiOutGetDevCapsW"));
	o_midiOutGetErrorTextA = reinterpret_cast<t_midiOutGetErrorTextA>(GetProcAddress(winmm_dll, "midiOutGetErrorTextA"));
	o_midiOutGetErrorTextW = reinterpret_cast<t_midiOutGetErrorTextW>(GetProcAddress(winmm_dll, "midiOutGetErrorTextW"));
	o_midiOutGetID = reinterpret_cast<t_midiOutGetID>(GetProcAddress(winmm_dll, "midiOutGetID"));
	o_midiOutGetNumDevs = reinterpret_cast<t_midiOutGetNumDevs>(GetProcAddress(winmm_dll, "midiOutGetNumDevs"));
	o_midiOutGetVolume = reinterpret_cast<t_midiOutGetVolume>(GetProcAddress(winmm_dll, "midiOutGetVolume"));
	o_midiOutLongMsg = reinterpret_cast<t_midiOutLongMsg>(GetProcAddress(winmm_dll, "midiOutLongMsg"));
	o_midiOutMessage = reinterpret_cast<t_midiOutMessage>(GetProcAddress(winmm_dll, "midiOutMessage"));
	o_midiOutOpen = reinterpret_cast<t_midiOutOpen>(GetProcAddress(winmm_dll, "midiOutOpen"));
	o_midiOutPrepareHeader = reinterpret_cast<t_midiOutPrepareHeader>(GetProcAddress(winmm_dll, "midiOutPrepareHeader"));
	o_midiOutReset = reinterpret_cast<t_midiOutReset>(GetProcAddress(winmm_dll, "midiOutReset"));
	o_midiOutSetVolume = reinterpret_cast<t_midiOutSetVolume>(GetProcAddress(winmm_dll, "midiOutSetVolume"));
	o_midiOutShortMsg = reinterpret_cast<t_midiOutShortMsg>(GetProcAddress(winmm_dll, "midiOutShortMsg"));
	o_midiOutUnprepareHeader = reinterpret_cast<t_midiOutUnprepareHeader>(GetProcAddress(winmm_dll, "midiOutUnprepareHeader"));
	o_midiStreamClose = reinterpret_cast<t_midiStreamClose>(GetProcAddress(winmm_dll, "midiStreamClose"));
	o_midiStreamOpen = reinterpret_cast<t_midiStreamOpen>(GetProcAddress(winmm_dll, "midiStreamOpen"));
	o_midiStreamOut = reinterpret_cast<t_midiStreamOut>(GetProcAddress(winmm_dll, "midiStreamOut"));
	o_midiStreamPause = reinterpret_cast<t_midiStreamPause>(GetProcAddress(winmm_dll, "midiStreamPause"));
	o_midiStreamPosition = reinterpret_cast<t_midiStreamPosition>(GetProcAddress(winmm_dll, "midiStreamPosition"));
	o_midiStreamProperty = reinterpret_cast<t_midiStreamProperty>(GetProcAddress(winmm_dll, "midiStreamProperty"));
	o_midiStreamRestart = reinterpret_cast<t_midiStreamRestart>(GetProcAddress(winmm_dll, "midiStreamRestart"));
	o_midiStreamStop = reinterpret_cast<t_midiStreamStop>(GetProcAddress(winmm_dll, "midiStreamStop"));
	o_mixerClose = reinterpret_cast<t_mixerClose>(GetProcAddress(winmm_dll, "mixerClose"));
	o_mixerGetControlDetailsA = reinterpret_cast<t_mixerGetControlDetailsA>(GetProcAddress(winmm_dll, "mixerGetControlDetailsA"));
	o_mixerGetControlDetailsW = reinterpret_cast<t_mixerGetControlDetailsW>(GetProcAddress(winmm_dll, "mixerGetControlDetailsW"));
	o_mixerGetDevCapsA = reinterpret_cast<t_mixerGetDevCapsA>(GetProcAddress(winmm_dll, "mixerGetDevCapsA"));
	o_mixerGetDevCapsW = reinterpret_cast<t_mixerGetDevCapsW>(GetProcAddress(winmm_dll, "mixerGetDevCapsW"));
	o_mixerGetID = reinterpret_cast<t_mixerGetID>(GetProcAddress(winmm_dll, "mixerGetID"));
	o_mixerGetLineControlsA = reinterpret_cast<t_mixerGetLineControlsA>(GetProcAddress(winmm_dll, "mixerGetLineControlsA"));
	o_mixerGetLineControlsW = reinterpret_cast<t_mixerGetLineControlsW>(GetProcAddress(winmm_dll, "mixerGetLineControlsW"));
	o_mixerGetLineInfoA = reinterpret_cast<t_mixerGetLineInfoA>(GetProcAddress(winmm_dll, "mixerGetLineInfoA"));
	o_mixerGetLineInfoW = reinterpret_cast<t_mixerGetLineInfoW>(GetProcAddress(winmm_dll, "mixerGetLineInfoW"));
	o_mixerGetNumDevs = reinterpret_cast<t_mixerGetNumDevs>(GetProcAddress(winmm_dll, "mixerGetNumDevs"));
	o_mixerMessage = reinterpret_cast<t_mixerMessage>(GetProcAddress(winmm_dll, "mixerMessage"));
	o_mixerOpen = reinterpret_cast<t_mixerOpen>(GetProcAddress(winmm_dll, "mixerOpen"));
	o_mixerSetControlDetails = reinterpret_cast<t_mixerSetControlDetails>(GetProcAddress(winmm_dll, "mixerSetControlDetails"));
	o_mmDrvInstall = reinterpret_cast<t_mmDrvInstall>(GetProcAddress(winmm_dll, "mmDrvInstall"));
	o_mmGetCurrentTask = reinterpret_cast<t_mmGetCurrentTask>(GetProcAddress(winmm_dll, "mmGetCurrentTask"));
	o_mmTaskBlock = reinterpret_cast<t_mmTaskBlock>(GetProcAddress(winmm_dll, "mmTaskBlock"));
	o_mmTaskCreate = reinterpret_cast<t_mmTaskCreate>(GetProcAddress(winmm_dll, "mmTaskCreate"));
	o_mmTaskSignal = reinterpret_cast<t_mmTaskSignal>(GetProcAddress(winmm_dll, "mmTaskSignal"));
	o_mmTaskYield = reinterpret_cast<t_mmTaskYield>(GetProcAddress(winmm_dll, "mmTaskYield"));
	o_mmioAdvance = reinterpret_cast<t_mmioAdvance>(GetProcAddress(winmm_dll, "mmioAdvance"));
	o_mmioAscend = reinterpret_cast<t_mmioAscend>(GetProcAddress(winmm_dll, "mmioAscend"));
	o_mmioClose = reinterpret_cast<t_mmioClose>(GetProcAddress(winmm_dll, "mmioClose"));
	o_mmioCreateChunk = reinterpret_cast<t_mmioCreateChunk>(GetProcAddress(winmm_dll, "mmioCreateChunk"));
	o_mmioDescend = reinterpret_cast<t_mmioDescend>(GetProcAddress(winmm_dll, "mmioDescend"));
	o_mmioFlush = reinterpret_cast<t_mmioFlush>(GetProcAddress(winmm_dll, "mmioFlush"));
	o_mmioGetInfo = reinterpret_cast<t_mmioGetInfo>(GetProcAddress(winmm_dll, "mmioGetInfo"));
	o_mmioInstallIOProcA = reinterpret_cast<t_mmioInstallIOProcA>(GetProcAddress(winmm_dll, "mmioInstallIOProcA"));
	o_mmioInstallIOProcW = reinterpret_cast<t_mmioInstallIOProcW>(GetProcAddress(winmm_dll, "mmioInstallIOProcW"));
	o_mmioOpenA = reinterpret_cast<t_mmioOpenA>(GetProcAddress(winmm_dll, "mmioOpenA"));
	o_mmioOpenW = reinterpret_cast<t_mmioOpenW>(GetProcAddress(winmm_dll, "mmioOpenW"));
	o_mmioRead = reinterpret_cast<t_mmioRead>(GetProcAddress(winmm_dll, "mmioRead"));
	o_mmioRenameA = reinterpret_cast<t_mmioRenameA>(GetProcAddress(winmm_dll, "mmioRenameA"));
	o_mmioRenameW = reinterpret_cast<t_mmioRenameW>(GetProcAddress(winmm_dll, "mmioRenameW"));
	o_mmioSeek = reinterpret_cast<t_mmioSeek>(GetProcAddress(winmm_dll, "mmioSeek"));
	o_mmioSendMessage = reinterpret_cast<t_mmioSendMessage>(GetProcAddress(winmm_dll, "mmioSendMessage"));
	o_mmioSetBuffer = reinterpret_cast<t_mmioSetBuffer>(GetProcAddress(winmm_dll, "mmioSetBuffer"));
	o_mmioSetInfo = reinterpret_cast<t_mmioSetInfo>(GetProcAddress(winmm_dll, "mmioSetInfo"));
	o_mmioStringToFOURCCA = reinterpret_cast<t_mmioStringToFOURCCA>(GetProcAddress(winmm_dll, "mmioStringToFOURCCA"));
	o_mmioStringToFOURCCW = reinterpret_cast<t_mmioStringToFOURCCW>(GetProcAddress(winmm_dll, "mmioStringToFOURCCW"));
	o_mmioWrite = reinterpret_cast<t_mmioWrite>(GetProcAddress(winmm_dll, "mmioWrite"));
	o_mmsystemGetVersion = reinterpret_cast<t_mmsystemGetVersion>(GetProcAddress(winmm_dll, "mmsystemGetVersion"));
	o_sndPlaySoundA = reinterpret_cast<t_sndPlaySoundA>(GetProcAddress(winmm_dll, "sndPlaySoundA"));
	o_sndPlaySoundW = reinterpret_cast<t_sndPlaySoundW>(GetProcAddress(winmm_dll, "sndPlaySoundW"));
	o_timeBeginPeriod = reinterpret_cast<t_timeBeginPeriod>(GetProcAddress(winmm_dll, "timeBeginPeriod"));
	o_timeEndPeriod = reinterpret_cast<t_timeEndPeriod>(GetProcAddress(winmm_dll, "timeEndPeriod"));
	o_timeGetDevCaps = reinterpret_cast<t_timeGetDevCaps>(GetProcAddress(winmm_dll, "timeGetDevCaps"));
	o_timeGetSystemTime = reinterpret_cast<t_timeGetSystemTime>(GetProcAddress(winmm_dll, "timeGetSystemTime"));
	o_timeGetTime = reinterpret_cast<t_timeGetTime>(GetProcAddress(winmm_dll, "timeGetTime"));
	o_timeKillEvent = reinterpret_cast<t_timeKillEvent>(GetProcAddress(winmm_dll, "timeKillEvent"));
	o_timeSetEvent = reinterpret_cast<t_timeSetEvent>(GetProcAddress(winmm_dll, "timeSetEvent"));
	o_waveInAddBuffer = reinterpret_cast<t_waveInAddBuffer>(GetProcAddress(winmm_dll, "waveInAddBuffer"));
	o_waveInClose = reinterpret_cast<t_waveInClose>(GetProcAddress(winmm_dll, "waveInClose"));
	o_waveInGetDevCapsA = reinterpret_cast<t_waveInGetDevCapsA>(GetProcAddress(winmm_dll, "waveInGetDevCapsA"));
	o_waveInGetDevCapsW = reinterpret_cast<t_waveInGetDevCapsW>(GetProcAddress(winmm_dll, "waveInGetDevCapsW"));
	o_waveInGetErrorTextA = reinterpret_cast<t_waveInGetErrorTextA>(GetProcAddress(winmm_dll, "waveInGetErrorTextA"));
	o_waveInGetErrorTextW = reinterpret_cast<t_waveInGetErrorTextW>(GetProcAddress(winmm_dll, "waveInGetErrorTextW"));
	o_waveInGetID = reinterpret_cast<t_waveInGetID>(GetProcAddress(winmm_dll, "waveInGetID"));
	o_waveInGetNumDevs = reinterpret_cast<t_waveInGetNumDevs>(GetProcAddress(winmm_dll, "waveInGetNumDevs"));
	o_waveInGetPosition = reinterpret_cast<t_waveInGetPosition>(GetProcAddress(winmm_dll, "waveInGetPosition"));
	o_waveInMessage = reinterpret_cast<t_waveInMessage>(GetProcAddress(winmm_dll, "waveInMessage"));
	o_waveInOpen = reinterpret_cast<t_waveInOpen>(GetProcAddress(winmm_dll, "waveInOpen"));
	o_waveInPrepareHeader = reinterpret_cast<t_waveInPrepareHeader>(GetProcAddress(winmm_dll, "waveInPrepareHeader"));
	o_waveInReset = reinterpret_cast<t_waveInReset>(GetProcAddress(winmm_dll, "waveInReset"));
	o_waveInStart = reinterpret_cast<t_waveInStart>(GetProcAddress(winmm_dll, "waveInStart"));
	o_waveInStop = reinterpret_cast<t_waveInStop>(GetProcAddress(winmm_dll, "waveInStop"));
	o_waveInUnprepareHeader = reinterpret_cast<t_waveInUnprepareHeader>(GetProcAddress(winmm_dll, "waveInUnprepareHeader"));
	o_waveOutBreakLoop = reinterpret_cast<t_waveOutBreakLoop>(GetProcAddress(winmm_dll, "waveOutBreakLoop"));
	o_waveOutClose = reinterpret_cast<t_waveOutClose>(GetProcAddress(winmm_dll, "waveOutClose"));
	o_waveOutGetDevCapsA = reinterpret_cast<t_waveOutGetDevCapsA>(GetProcAddress(winmm_dll, "waveOutGetDevCapsA"));
	o_waveOutGetDevCapsW = reinterpret_cast<t_waveOutGetDevCapsW>(GetProcAddress(winmm_dll, "waveOutGetDevCapsW"));
	o_waveOutGetErrorTextA = reinterpret_cast<t_waveOutGetErrorTextA>(GetProcAddress(winmm_dll, "waveOutGetErrorTextA"));
	o_waveOutGetErrorTextW = reinterpret_cast<t_waveOutGetErrorTextW>(GetProcAddress(winmm_dll, "waveOutGetErrorTextW"));
	o_waveOutGetID = reinterpret_cast<t_waveOutGetID>(GetProcAddress(winmm_dll, "waveOutGetID"));
	o_waveOutGetNumDevs = reinterpret_cast<t_waveOutGetNumDevs>(GetProcAddress(winmm_dll, "waveOutGetNumDevs"));
	o_waveOutGetPitch = reinterpret_cast<t_waveOutGetPitch>(GetProcAddress(winmm_dll, "waveOutGetPitch"));
	o_waveOutGetPlaybackRate = reinterpret_cast<t_waveOutGetPlaybackRate>(GetProcAddress(winmm_dll, "waveOutGetPlaybackRate"));
	o_waveOutGetPosition = reinterpret_cast<t_waveOutGetPosition>(GetProcAddress(winmm_dll, "waveOutGetPosition"));
	o_waveOutGetVolume = reinterpret_cast<t_waveOutGetVolume>(GetProcAddress(winmm_dll, "waveOutGetVolume"));
	o_waveOutMessage = reinterpret_cast<t_waveOutMessage>(GetProcAddress(winmm_dll, "waveOutMessage"));
	o_waveOutOpen = reinterpret_cast<t_waveOutOpen>(GetProcAddress(winmm_dll, "waveOutOpen"));
	o_waveOutPause = reinterpret_cast<t_waveOutPause>(GetProcAddress(winmm_dll, "waveOutPause"));
	o_waveOutPrepareHeader = reinterpret_cast<t_waveOutPrepareHeader>(GetProcAddress(winmm_dll, "waveOutPrepareHeader"));
	o_waveOutReset = reinterpret_cast<t_waveOutReset>(GetProcAddress(winmm_dll, "waveOutReset"));
	o_waveOutRestart = reinterpret_cast<t_waveOutRestart>(GetProcAddress(winmm_dll, "waveOutRestart"));
	o_waveOutSetPitch = reinterpret_cast<t_waveOutSetPitch>(GetProcAddress(winmm_dll, "waveOutSetPitch"));
	o_waveOutSetPlaybackRate = reinterpret_cast<t_waveOutSetPlaybackRate>(GetProcAddress(winmm_dll, "waveOutSetPlaybackRate"));
	o_waveOutSetVolume = reinterpret_cast<t_waveOutSetVolume>(GetProcAddress(winmm_dll, "waveOutSetVolume"));
	o_waveOutUnprepareHeader = reinterpret_cast<t_waveOutUnprepareHeader>(GetProcAddress(winmm_dll, "waveOutUnprepareHeader"));
	o_waveOutWrite = reinterpret_cast<t_waveOutWrite>(GetProcAddress(winmm_dll, "waveOutWrite"));
}