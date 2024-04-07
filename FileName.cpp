
LRESULT WINAPI _CloseDriver(HDRVR hDriver, LPARAM lParam1, LPARAM lParam2)
{
    return CloseDriver(hDriver, lParam1, lParam2);
}

HDRVR WINAPI _OpenDriver(LPCWSTR szDriverName, LPCWSTR szSectionName, LPARAM lParam2)
{
    return OpenDriver(szDriverName, szSectionName, lParam2);
}

LRESULT WINAPI _SendDriverMessage(HDRVR hDriver, UINT message, LPARAM lParam1, LPARAM lParam2)
{
    return SendDriverMessage(hDriver, message, lParam1, lParam2);
}

HMODULE WINAPI _DrvGetModuleHandle(HDRVR hDriver)
{
    return DrvGetModuleHandle(hDriver);
}

HMODULE WINAPI _GetDriverModuleHandle(HDRVR hDriver)
{
    return GetDriverModuleHandle(hDriver);
}

LRESULT WINAPI _DefDriverProc(DWORD_PTR dwDriverIdentifier, HDRVR hdrvr, UINT uMsg, LPARAM lParam1, LPARAM lParam2)
{
    return DefDriverProc(dwDriverIdentifier, hdrvr, uMsg, lParam1, lParam2);
}

BOOL APIENTRY _DriverCallback(DWORD_PTR dwCallback, DWORD dwFlags, HDRVR hDevice, DWORD dwMsg, DWORD_PTR dwUser,
    DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    return DriverCallback(dwCallback, dwFlags, hDevice, dwMsg, dwUser, dwParam1, dwParam2);
}

BOOL WINAPI _PlaySound(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
    return PlaySound(pszSound, hmod, fdwSound);
}

BOOL WINAPI _PlaySoundA(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
    return PlaySoundA(pszSound, hmod, fdwSound);
}

BOOL WINAPI _PlaySoundW(LPCWSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
    return PlaySoundW(pszSound, hmod, fdwSound);
}

UINT WINAPI _auxGetNumDevs(void)
{
    return auxGetNumDevs();
}

MMRESULT WINAPI _auxGetDevCapsA(UINT_PTR uDeviceID, LPAUXCAPSA pac, UINT cbac)
{
    return auxGetDevCapsA(uDeviceID, pac, cbac);
}

MMRESULT WINAPI _auxGetDevCapsW(UINT_PTR uDeviceID, LPAUXCAPSW pac, UINT cbac)
{
    return auxGetDevCapsW(uDeviceID, pac, cbac);
}

MMRESULT WINAPI _auxSetVolume(UINT uDeviceID, DWORD dwVolume)
{
    return auxSetVolume(uDeviceID, dwVolume);
}

MMRESULT WINAPI _auxGetVolume(UINT uDeviceID, LPDWORD pdwVolume)
{
    return auxGetVolume(uDeviceID, pdwVolume);
}

MMRESULT WINAPI _auxOutMessage(UINT uDeviceID, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
    return auxOutMessage(uDeviceID, uMsg, dw1, dw2);
}

MMRESULT WINAPI _joyGetPos(UINT uJoyID, LPJOYINFO pji)
{
    return joyGetPos(uJoyID, pji);
}

MMRESULT WINAPI _joyGetThreshold(UINT uJoyID, LPUINT puThreshold)
{
    return joyGetThreshold(uJoyID, puThreshold);
}

MMRESULT WINAPI _joyReleaseCapture(UINT uJoyID)
{
    return joyReleaseCapture(uJoyID);
}

MMRESULT WINAPI _joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged)
{
    return joySetCapture(hwnd, uJoyID, uPeriod, fChanged);
}

MMRESULT WINAPI _joySetThreshold(UINT uJoyID, UINT uThreshold)
{
    return joySetThreshold(uJoyID, uThreshold);
}

MMRESULT WINAPI _joyConfigChanged(DWORD dwFlags)
{
    return joyConfigChanged(dwFlags);
}

MMRESULT WINAPI _joyGetPosEx(UINT uJoyID, LPJOYINFOEX pji)
{
    return joyGetPosEx(uJoyID, pji);
}

UINT WINAPI _joyGetNumDevs(void)
{
    return joyGetNumDevs();
}

MMRESULT WINAPI _joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc)
{
    return joyGetDevCapsA(uJoyID, pjc, cbjc);
}

MMRESULT WINAPI _joyGetDevCapsW(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc)
{
    return joyGetDevCapsW(uJoyID, pjc, cbjc);
}

MCIERROR WINAPI _mciSendCommandA(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    return mciSendCommandA(mciId, uMsg, dwParam1, dwParam2);
}

MCIERROR WINAPI _mciSendCommandW(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    return mciSendCommandW(mciId, uMsg, dwParam1, dwParam2);
}

MCIERROR WINAPI _mciSendStringA(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback)
{
    return mciSendStringA(lpstrCommand, lpstrReturnString, uReturnLength, hwndCallback);
}

MCIERROR WINAPI _mciSendStringW(LPCWSTR lpstrCommand, LPWSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback)
{
    return mciSendStringW(lpstrCommand, lpstrReturnString, uReturnLength, hwndCallback);
}

MCIDEVICEID WINAPI _mciGetDeviceIDA(LPCSTR pszDevice)
{
    return mciGetDeviceIDA(pszDevice);
}

MCIDEVICEID WINAPI _mciGetDeviceIDW(LPCWSTR pszDevice)
{
    return mciGetDeviceIDW(pszDevice);
}

MCIDEVICEID WINAPI _mciGetDeviceIDFromElementIDA(DWORD dwElementID, LPCSTR lpstrType)
{
    return mciGetDeviceIDFromElementIDA(dwElementID, lpstrType);
}

MCIDEVICEID WINAPI _mciGetDeviceIDFromElementIDW(DWORD dwElementID, LPCWSTR lpstrType)
{
    return mciGetDeviceIDFromElementIDW(dwElementID, lpstrType);
}

BOOL WINAPI _mciGetErrorStringA(MCIERROR mcierr, LPSTR pszText, UINT cchText)
{
    return mciGetErrorStringA(mcierr, pszText, cchText);
}

BOOL WINAPI _mciGetErrorStringW(MCIERROR mcierr, LPWSTR pszText, UINT cchText)
{
    return mciGetErrorStringW(mcierr, pszText, cchText);
}

BOOL WINAPI _mciSetYieldProc(MCIDEVICEID mciId, YIELDPROC fpYieldProc, DWORD dwYieldData)
{
    return mciSetYieldProc(mciId, fpYieldProc, dwYieldData);
}

HTASK WINAPI _mciGetCreatorTask(MCIDEVICEID mciId)
{
    return mciGetCreatorTask(mciId);
}

YIELDPROC WINAPI _mciGetYieldProc(MCIDEVICEID mciId, LPDWORD pdwYieldData)
{
    return mciGetYieldProc(mciId, pdwYieldData);
}

DWORD_PTR APIENTRY _mciGetDriverData(MCIDEVICEID wDeviceID)
{
    return mciGetDriverData(wDeviceID);
}

UINT APIENTRY _mciLoadCommandResource(HANDLE hInstance, LPCWSTR lpResName, UINT wType)
{
    return mciLoadCommandResource(hInstance, lpResName, wType);
}

BOOL APIENTRY _mciSetDriverData(MCIDEVICEID wDeviceID, DWORD_PTR dwData)
{
    return mciSetDriverData(wDeviceID, dwData);
}

UINT APIENTRY _mciDriverYield(MCIDEVICEID wDeviceID)
{
    return mciDriverYield(wDeviceID);
}

BOOL APIENTRY _mciDriverNotify(HANDLE hwndCallback, MCIDEVICEID wDeviceID, UINT uStatus)
{
    return mciDriverNotify(hwndCallback, wDeviceID, uStatus);
}

BOOL APIENTRY _mciFreeCommandResource(UINT wTable)
{
    return mciFreeCommandResource(wTable);
}

UINT WINAPI _midiOutGetNumDevs(void)
{
    return midiOutGetNumDevs();
}

MMRESULT WINAPI _midiStreamOpen(LPHMIDISTRM phms, LPUINT puDeviceID, DWORD cMidi, DWORD_PTR dwCallback,
    DWORD_PTR dwInstance, DWORD fdwOpen)
{
    return midiStreamOpen(phms, puDeviceID, cMidi, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI _midiStreamClose(HMIDISTRM hms)
{
    return midiStreamClose(hms);
}

MMRESULT WINAPI _midiStreamProperty(HMIDISTRM hms, LPBYTE lppropdata, DWORD dwProperty)
{
    return midiStreamProperty(hms, lppropdata, dwProperty);
}

MMRESULT WINAPI _midiStreamPosition(HMIDISTRM hms, LPMMTIME lpmmt, UINT cbmmt)
{
    return midiStreamPosition(hms, lpmmt, cbmmt);
}

MMRESULT WINAPI _midiStreamOut(HMIDISTRM hms, LPMIDIHDR pmh, UINT cbmh)
{
    return midiStreamOut(hms, pmh, cbmh);
}

MMRESULT WINAPI _midiStreamPause(HMIDISTRM hms)
{
    return midiStreamPause(hms);
}

MMRESULT WINAPI _midiStreamRestart(HMIDISTRM hms)
{
    return midiStreamRestart(hms);
}

MMRESULT WINAPI _midiStreamStop(HMIDISTRM hms)
{
    return midiStreamStop(hms);
}

MMRESULT WINAPI _midiConnect(HMIDI hmi, HMIDIOUT hmo, LPVOID pReserved)
{
    return midiConnect(hmi, hmo, pReserved);
}

MMRESULT WINAPI _midiDisconnect(HMIDI hmi, HMIDIOUT hmo, LPVOID pReserved)
{
    return midiDisconnect(hmi, hmo, pReserved);
}

MMRESULT WINAPI _midiOutGetDevCapsA(UINT_PTR uDeviceID, LPMIDIOUTCAPSA pmoc, UINT cbmoc)
{
    return midiOutGetDevCapsA(uDeviceID, pmoc, cbmoc);
}

MMRESULT WINAPI _midiOutGetDevCapsW(UINT_PTR uDeviceID, LPMIDIOUTCAPSW pmoc, UINT cbmoc)
{
    return midiOutGetDevCapsW(uDeviceID, pmoc, cbmoc);
}

MMRESULT WINAPI _midiOutGetVolume(HMIDIOUT hmo, LPDWORD pdwVolume)
{
    return midiOutGetVolume(hmo, pdwVolume);
}

MMRESULT WINAPI _midiOutSetVolume(HMIDIOUT hmo, DWORD dwVolume)
{
    return midiOutSetVolume(hmo, dwVolume);
}

MMRESULT WINAPI _midiOutGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText)
{
    return midiOutGetErrorTextA(mmrError, pszText, cchText);
}

MMRESULT WINAPI _midiOutGetErrorTextW(MMRESULT mmrError, LPWSTR pszText, UINT cchText)
{
    return midiOutGetErrorTextW(mmrError, pszText, cchText);
}

MMRESULT WINAPI _midiOutOpen(LPHMIDIOUT phmo, UINT uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
    return midiOutOpen(phmo, uDeviceID, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI _midiOutClose(HMIDIOUT hmo)
{
    return midiOutClose(hmo);
}

MMRESULT WINAPI _midiOutPrepareHeader(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh)
{
    return midiOutPrepareHeader(hmo, pmh, cbmh);
}

MMRESULT WINAPI _midiOutUnprepareHeader(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh)
{
    return midiOutUnprepareHeader(hmo, pmh, cbmh);
}

MMRESULT WINAPI _midiOutShortMsg(HMIDIOUT hmo, DWORD dwMsg)
{
    return midiOutShortMsg(hmo, dwMsg);
}

MMRESULT WINAPI _midiOutLongMsg(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh)
{
    return midiOutLongMsg(hmo, pmh, cbmh);
}

MMRESULT WINAPI _midiOutReset(HMIDIOUT hmo)
{
    return midiOutReset(hmo);
}

MMRESULT WINAPI _midiOutCachePatches(HMIDIOUT hmo, UINT uBank, LPWORD pwpa, UINT fuCache)
{
    return midiOutCachePatches(hmo, uBank, pwpa, fuCache);
}

MMRESULT WINAPI _midiOutCacheDrumPatches(HMIDIOUT hmo, UINT uPatch, LPWORD pwkya, UINT fuCache)
{
    return midiOutCacheDrumPatches(hmo, uPatch, pwkya, fuCache);
}

MMRESULT WINAPI _midiOutGetID(HMIDIOUT hmo, LPUINT puDeviceID)
{
    return midiOutGetID(hmo, puDeviceID);
}

MMRESULT WINAPI _midiOutMessage(HMIDIOUT hmo, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
    return midiOutMessage(hmo, uMsg, dw1, dw2);
}

UINT WINAPI _midiInGetNumDevs(void)
{
    return midiInGetNumDevs();
}

MMRESULT WINAPI _midiInGetDevCapsA(UINT_PTR uDeviceID, LPMIDIINCAPSA pmic, UINT cbmic)
{
    return midiInGetDevCapsA(uDeviceID, pmic, cbmic);
}

MMRESULT WINAPI _midiInGetDevCapsW(UINT_PTR uDeviceID, LPMIDIINCAPSW pmic, UINT cbmic)
{
    return midiInGetDevCapsW(uDeviceID, pmic, cbmic);
}

MMRESULT WINAPI _midiInGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText)
{
    return midiInGetErrorTextA(mmrError, pszText, cchText);
}

MMRESULT WINAPI _midiInGetErrorTextW(MMRESULT mmrError, LPWSTR pszText, UINT cchText)
{
    return midiInGetErrorTextW(mmrError, pszText, cchText);
}

MMRESULT WINAPI _midiInOpen(LPHMIDIIN phmi, UINT uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
    return midiInOpen(phmi, uDeviceID, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI _midiInClose(HMIDIIN hmi)
{
    return midiInClose(hmi);
}

MMRESULT WINAPI _midiInPrepareHeader(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh)
{
    return midiInPrepareHeader(hmi, pmh, cbmh);
}

MMRESULT WINAPI _midiInUnprepareHeader(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh)
{
    return midiInUnprepareHeader(hmi, pmh, cbmh);
}

MMRESULT WINAPI _midiInAddBuffer(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh)
{
    return midiInAddBuffer(hmi, pmh, cbmh);
}

MMRESULT WINAPI _midiInStart(HMIDIIN hmi)
{
    return midiInStart(hmi);
}

MMRESULT WINAPI _midiInStop(HMIDIIN hmi)
{
    return midiInStop(hmi);
}

MMRESULT WINAPI _midiInReset(HMIDIIN hmi)
{
    return midiInReset(hmi);
}

MMRESULT WINAPI _midiInGetID(HMIDIIN hmi, LPUINT puDeviceID)
{
    return midiInGetID(hmi, puDeviceID);
}

MMRESULT WINAPI _midiInMessage(HMIDIIN hmi, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
    return midiInMessage(hmi, uMsg, dw1, dw2);
}

UINT WINAPI _mixerGetNumDevs(void)
{
    return mixerGetNumDevs();
}

MMRESULT WINAPI _mixerGetDevCapsA(UINT_PTR uMxId, LPMIXERCAPSA pmxcaps, UINT cbmxcaps)
{
    return mixerGetDevCapsA(uMxId, pmxcaps, cbmxcaps);
}

MMRESULT WINAPI _mixerGetDevCapsW(UINT_PTR uMxId, LPMIXERCAPSW pmxcaps, UINT cbmxcaps)
{
    return mixerGetDevCapsW(uMxId, pmxcaps, cbmxcaps);
}

MMRESULT WINAPI _mixerOpen(LPHMIXER phmx, UINT uMxId, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
    return mixerOpen(phmx, uMxId, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI _mixerClose(HMIXER hmx)
{
    return mixerClose(hmx);
}

DWORD WINAPI _mixerMessage(HMIXER hmx, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    return mixerMessage(hmx, uMsg, dwParam1, dwParam2);
}

MMRESULT WINAPI _mixerGetLineInfoA(HMIXEROBJ hmxobj, LPMIXERLINEA pmxl, DWORD fdwInfo)
{
    return mixerGetLineInfoA(hmxobj, pmxl, fdwInfo);
}

MMRESULT WINAPI _mixerGetLineInfoW(HMIXEROBJ hmxobj, LPMIXERLINEW pmxl, DWORD fdwInfo)
{
    return mixerGetLineInfoW(hmxobj, pmxl, fdwInfo);
}

MMRESULT WINAPI _mixerGetID(HMIXEROBJ hmxobj, UINT FAR* puMxId, DWORD fdwId)
{
    return mixerGetID(hmxobj, puMxId, fdwId);
}

MMRESULT WINAPI _mixerGetLineControlsA(HMIXEROBJ hmxobj, LPMIXERLINECONTROLSA pmxlc, DWORD fdwControls)
{
    return mixerGetLineControlsA(hmxobj, pmxlc, fdwControls);
}

MMRESULT WINAPI _mixerGetLineControlsW(HMIXEROBJ hmxobj, LPMIXERLINECONTROLSW pmxlc, DWORD fdwControls)
{
    return mixerGetLineControlsW(hmxobj, pmxlc, fdwControls);
}

MMRESULT WINAPI _mixerGetControlDetailsA(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails)
{
    return mixerGetControlDetailsA(hmxobj, pmxcd, fdwDetails);
}

MMRESULT WINAPI _mixerGetControlDetailsW(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails)
{
    return mixerGetControlDetailsW(hmxobj, pmxcd, fdwDetails);
}

MMRESULT WINAPI _mixerSetControlDetails(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails)
{
    return mixerSetControlDetails(hmxobj, pmxcd, fdwDetails);
}

UINT APIENTRY _mmDrvInstall(HDRVR hDriver, LPCWSTR wszDrvEntry, DRIVERMSGPROC drvMessage, UINT wFlags)
{
    return mmDrvInstall(hDriver, wszDrvEntry, drvMessage, wFlags);
}

UINT APIENTRY _mmTaskCreate(LPTASKCALLBACK lpfn, HANDLE FAR* lph, DWORD_PTR dwInst)
{
    return mmTaskCreate(lpfn, lph, dwInst);
}

VOID APIENTRY _mmTaskBlock(DWORD h)
{
    return mmTaskBlock(h);
}

BOOL APIENTRY _mmTaskSignal(DWORD h)
{
    return mmTaskSignal(h);
}

VOID APIENTRY _mmTaskYield(VOID)
{
    return mmTaskYield();
}

DWORD APIENTRY _mmGetCurrentTask(VOID)
{
    return mmGetCurrentTask();
}

FOURCC WINAPI _mmioStringToFOURCCA(LPCSTR sz, UINT uFlags)
{
    return mmioStringToFOURCCA(sz, uFlags);
}

FOURCC WINAPI _mmioStringToFOURCCW(LPCWSTR sz, UINT uFlags)
{
    return mmioStringToFOURCCW(sz, uFlags);
}

LPMMIOPROC WINAPI _mmioInstallIOProcA(FOURCC fccIOProc, LPMMIOPROC pIOProc, DWORD dwFlags)
{
    return mmioInstallIOProcA(fccIOProc, pIOProc, dwFlags);
}

LPMMIOPROC WINAPI _mmioInstallIOProcW(FOURCC fccIOProc, LPMMIOPROC pIOProc, DWORD dwFlags)
{
    return mmioInstallIOProcW(fccIOProc, pIOProc, dwFlags);
}

HMMIO WINAPI _mmioOpenA(LPSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen)
{
    return mmioOpenA(pszFileName, pmmioinfo, fdwOpen);
}

HMMIO WINAPI _mmioOpenW(LPWSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen)
{
    return mmioOpenW(pszFileName, pmmioinfo, fdwOpen);
}

MMRESULT WINAPI _mmioRenameA(LPCSTR pszFileName, LPCSTR pszNewFileName, LPCMMIOINFO pmmioinfo, DWORD fdwRename)
{
    return mmioRenameA(pszFileName, pszNewFileName, pmmioinfo, fdwRename);
}

MMRESULT WINAPI _mmioRenameW(LPCWSTR pszFileName, LPCWSTR pszNewFileName, LPCMMIOINFO pmmioinfo, DWORD fdwRename)
{
    return mmioRenameW(pszFileName, pszNewFileName, pmmioinfo, fdwRename);
}

MMRESULT WINAPI _mmioClose(HMMIO hmmio, UINT fuClose)
{
    return mmioClose(hmmio, fuClose);
}

LONG WINAPI _mmioRead(HMMIO hmmio, HPSTR pch, LONG cch)
{
    return mmioRead(hmmio, pch, cch);
}

LONG WINAPI _mmioWrite(HMMIO hmmio, const char _huge* pch, LONG cch)
{
    return mmioWrite(hmmio, pch, cch);
}

LONG WINAPI _mmioSeek(HMMIO hmmio, LONG lOffset, int iOrigin)
{
    return mmioSeek(hmmio, lOffset, iOrigin);
}

MMRESULT WINAPI _mmioGetInfo(HMMIO hmmio, LPMMIOINFO pmmioinfo, UINT fuInfo)
{
    return mmioGetInfo(hmmio, pmmioinfo, fuInfo);
}

MMRESULT WINAPI _mmioSetInfo(HMMIO hmmio, LPCMMIOINFO pmmioinfo, UINT fuInfo)
{
    return mmioSetInfo(hmmio, pmmioinfo, fuInfo);
}

MMRESULT WINAPI _mmioSetBuffer(HMMIO hmmio, LPSTR pchBuffer, LONG cchBuffer, UINT fuBuffer)
{
    return mmioSetBuffer(hmmio, pchBuffer, cchBuffer, fuBuffer);
}

MMRESULT WINAPI _mmioFlush(HMMIO hmmio, UINT fuFlush)
{
    return mmioFlush(hmmio, fuFlush);
}

MMRESULT WINAPI _mmioAdvance(HMMIO hmmio, LPMMIOINFO pmmioinfo, UINT fuAdvance)
{
    return mmioAdvance(hmmio, pmmioinfo, fuAdvance);
}

LRESULT WINAPI _mmioSendMessage(HMMIO hmmio, UINT uMsg, LPARAM lParam1, LPARAM lParam2)
{
    return mmioSendMessage(hmmio, uMsg, lParam1, lParam2);
}

MMRESULT WINAPI _mmioDescend(HMMIO hmmio, LPMMCKINFO pmmcki, const MMCKINFO FAR* pmmckiParent, UINT fuDescend)
{
    return mmioDescend(hmmio, pmmcki, pmmckiParent, fuDescend);
}

MMRESULT WINAPI _mmioAscend(HMMIO hmmio, LPMMCKINFO pmmcki, UINT fuAscend)
{
    return mmioAscend(hmmio, pmmcki, fuAscend);
}

MMRESULT WINAPI _mmioCreateChunk(HMMIO hmmio, LPMMCKINFO pmmcki, UINT fuCreate)
{
    return mmioCreateChunk(hmmio, pmmcki, fuCreate);
}

BOOL WINAPI _sndPlaySoundA(LPCSTR pszSound, UINT fuSound)
{
    return sndPlaySoundA(pszSound, fuSound);
}

BOOL WINAPI _sndPlaySoundW(LPCWSTR pszSound, UINT fuSound)
{
    return sndPlaySoundW(pszSound, fuSound);
}

MMRESULT WINAPI _timeGetSystemTime(LPMMTIME pmmt, UINT cbmmt)
{
    return timeGetSystemTime(pmmt, cbmmt);
}

DWORD WINAPI _timeGetTime(void)
{
    return timeGetTime();
}

MMRESULT WINAPI _timeGetDevCaps(LPTIMECAPS ptc, UINT cbtc)
{
    return timeGetDevCaps(ptc, cbtc);
}

MMRESULT WINAPI _timeBeginPeriod(UINT uPeriod)
{
    return timeBeginPeriod(uPeriod);
}

MMRESULT WINAPI _timeEndPeriod(UINT uPeriod)
{
    return timeEndPeriod(uPeriod);
}

MMRESULT WINAPI _timeSetEvent(UINT uDelay, UINT uResolution, LPTIMECALLBACK fptc, DWORD_PTR dwUser, UINT fuEvent)
{
    return timeSetEvent(uDelay, uResolution, fptc, dwUser, fuEvent);
}

MMRESULT WINAPI _timeKillEvent(UINT uTimerID)
{
    return timeKillEvent(uTimerID);
}

UINT WINAPI _waveOutGetNumDevs(void)
{
    return waveOutGetNumDevs();
}

MMRESULT WINAPI _waveOutGetDevCapsA(UINT_PTR uDeviceID, LPWAVEOUTCAPSA pwoc, UINT cbwoc)
{
    return waveOutGetDevCapsA(uDeviceID, pwoc, cbwoc);
}

MMRESULT WINAPI _waveOutGetDevCapsW(UINT_PTR uDeviceID, LPWAVEOUTCAPSW pwoc, UINT cbwoc)
{
    return waveOutGetDevCapsW(uDeviceID, pwoc, cbwoc);
}

MMRESULT WINAPI _waveOutGetVolume(HWAVEOUT hwo, LPDWORD pdwVolume)
{
    return waveOutGetVolume(hwo, pdwVolume);
}

MMRESULT WINAPI _waveOutSetVolume(HWAVEOUT hwo, DWORD dwVolume)
{
    return waveOutSetVolume(hwo, dwVolume);
}

MMRESULT WINAPI _waveOutGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText)
{
    return waveOutGetErrorTextA(mmrError, pszText, cchText);
}

MMRESULT WINAPI _waveOutGetErrorTextW(MMRESULT mmrError, LPWSTR pszText, UINT cchText)
{
    return waveOutGetErrorTextW(mmrError, pszText, cchText);
}

MMRESULT WINAPI _waveOutOpen(LPHWAVEOUT phwo, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback,
    DWORD_PTR dwInstance, DWORD fdwOpen)
{
    return waveOutOpen(phwo, uDeviceID, pwfx, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI _waveOutClose(HWAVEOUT hwo)
{
    return waveOutClose(hwo);
}

MMRESULT WINAPI _waveOutPrepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
    return waveOutPrepareHeader(hwo, pwh, cbwh);
}

MMRESULT WINAPI _waveOutUnprepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
    return waveOutUnprepareHeader(hwo, pwh, cbwh);
}

MMRESULT WINAPI _waveOutWrite(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
    return waveOutWrite(hwo, pwh, cbwh);
}

MMRESULT WINAPI _waveOutPause(HWAVEOUT hwo)
{
    return waveOutPause(hwo);
}

MMRESULT WINAPI _waveOutRestart(HWAVEOUT hwo)
{
    return waveOutRestart(hwo);
}

MMRESULT WINAPI _waveOutReset(HWAVEOUT hwo)
{
    return waveOutReset(hwo);
}

MMRESULT WINAPI _waveOutBreakLoop(HWAVEOUT hwo)
{
    return waveOutBreakLoop(hwo);
}

MMRESULT WINAPI _waveOutGetPosition(HWAVEOUT hwo, LPMMTIME pmmt, UINT cbmmt)
{
    return waveOutGetPosition(hwo, pmmt, cbmmt);
}

MMRESULT WINAPI _waveOutGetPitch(HWAVEOUT hwo, LPDWORD pdwPitch)
{
    return waveOutGetPitch(hwo, pdwPitch);
}

MMRESULT WINAPI _waveOutSetPitch(HWAVEOUT hwo, DWORD dwPitch)
{
    return waveOutSetPitch(hwo, dwPitch);
}

MMRESULT WINAPI _waveOutGetPlaybackRate(HWAVEOUT hwo, LPDWORD pdwRate)
{
    return waveOutGetPlaybackRate(hwo, pdwRate);
}

MMRESULT WINAPI _waveOutSetPlaybackRate(HWAVEOUT hwo, DWORD dwRate)
{
    return waveOutSetPlaybackRate(hwo, dwRate);
}

MMRESULT WINAPI _waveOutGetID(HWAVEOUT hwo, LPUINT puDeviceID)
{
    return waveOutGetID(hwo, puDeviceID);
}

MMRESULT WINAPI _waveOutMessage(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
    return waveOutMessage(hwo, uMsg, dw1, dw2);
}

UINT WINAPI _waveInGetNumDevs(void)
{
    return waveInGetNumDevs();
}

MMRESULT WINAPI _waveInGetDevCapsA(UINT_PTR uDeviceID, LPWAVEINCAPSA pwic, UINT cbwic)
{
    return waveInGetDevCapsA(uDeviceID, pwic, cbwic);
}

MMRESULT WINAPI _waveInGetDevCapsW(UINT_PTR uDeviceID, LPWAVEINCAPSW pwic, UINT cbwic)
{
    return waveInGetDevCapsW(uDeviceID, pwic, cbwic);
}

MMRESULT WINAPI _waveInGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText)
{
    return waveInGetErrorTextA(mmrError, pszText, cchText);
}

MMRESULT WINAPI _waveInGetErrorTextW(MMRESULT mmrError, LPWSTR pszText, UINT cchText)
{
    return waveInGetErrorTextW(mmrError, pszText, cchText);
}

MMRESULT WINAPI _waveInOpen(LPHWAVEIN phwi, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback,
    DWORD_PTR dwInstance, DWORD fdwOpen)
{
    return waveInOpen(phwi, uDeviceID, pwfx, dwCallback, dwInstance, fdwOpen);
}

MMRESULT WINAPI _waveInClose(HWAVEIN hwi)
{
    return waveInClose(hwi);
}

MMRESULT WINAPI _waveInPrepareHeader(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh)
{
    return waveInPrepareHeader(hwi, pwh, cbwh);
}

MMRESULT WINAPI _waveInUnprepareHeader(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh)
{
    return waveInUnprepareHeader(hwi, pwh, cbwh);
}

MMRESULT WINAPI _waveInAddBuffer(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh)
{
    return waveInAddBuffer(hwi, pwh, cbwh);
}

MMRESULT WINAPI _waveInStart(HWAVEIN hwi)
{
    return waveInStart(hwi);
}

MMRESULT WINAPI _waveInStop(HWAVEIN hwi)
{
    return waveInStop(hwi);
}

MMRESULT WINAPI _waveInReset(HWAVEIN hwi)
{
    return waveInReset(hwi);
}

MMRESULT WINAPI _waveInGetPosition(HWAVEIN hwi, LPMMTIME pmmt, UINT cbmmt)
{
    return waveInGetPosition(hwi, pmmt, cbmmt);
}

MMRESULT WINAPI _waveInGetID(HWAVEIN hwi, LPUINT puDeviceID)
{
    return waveInGetID(hwi, puDeviceID);
}

MMRESULT WINAPI _waveInMessage(HWAVEIN hwi, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2)
{
    return waveInMessage(hwi, uMsg, dw1, dw2);
}

VOID WINAPI _WOWAppExit()
{
    return WOWAppExit();
}

BOOL WINAPI _mciExecute(LPCSTR pszCommand)
{
    return mciExecute(pszCommand);
}

UINT WINAPI _mmsystemGetVersion()
{
    return mmsystemGetVersion();
}

BOOL WINAPI _PlaySoundStub(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound)
{
    return PlaySoundStub(pszSound, hmod, fdwSound);
}
