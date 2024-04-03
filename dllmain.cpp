// dllmain.cpp : Defines the entry point for the DLL application.
#include "common.h"
#include "Hooks.h"
#include "global.h"
#include <mmeapi.h>

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

void setup_winmm() {
	mciExecute_Real = (mciexecute)GetProcAddress(winmm_dll, "mciExecute");
	CloseDriver_Real = (closedriver)GetProcAddress(winmm_dll, "CloseDriver");
	/*
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
	auxOutMessage_Real = (auxOutMessage)GetProcAddress(winmm_dll, "auxOutMessage");
	auxSetVolume_Real = (auxSetVolume)GetProcAddress(winmm_dll, "auxSetVolume");
	joyConfigChanged_Real = (joyConfigChanged)GetProcAddress(winmm_dll, "joyConfigChanged");
	joyGetDevCapsA_Real = (joyGetDevCapsA)GetProcAddress(winmm_dll, "joyGetDevCapsA");
	joyGetDevCapsW_Real = (joyGetDevCapsW)GetProcAddress(winmm_dll, "joyGetDevCapsW");
	joyGetNumDevs_Real = (joyGetNumDevs)GetProcAddress(winmm_dll, "joyGetNumDevs");
	joyGetPos_Real = (joyGetPos)GetProcAddress(winmm_dll, "joyGetPos");
	joyGetPosEx_Real = (joyGetPosEx)GetProcAddress(winmm_dll, "joyGetPosEx");
	joyGetThreshold_Real = (joyGetThreshold)GetProcAddress(winmm_dll, "joyGetThreshold");
	joyReleaseCapture_Real = (joyReleaseCapture)GetProcAddress(winmm_dll, "joyReleaseCapture");
	joySetCapture_Real = (joySetCapture)GetProcAddress(winmm_dll, "joySetCapture");
	joySetThreshold_Real = (joySetThreshold)GetProcAddress(winmm_dll, "joySetThreshold");
	mciDriverNotify_Real = (mciDriverNotify)GetProcAddress(winmm_dll, "mciDriverNotify");
	mciDriverYield_Real = (")GetProcAddress(winmm_dll, "mciDriverYield");
		mciFreeCommandResource_Real = (mciFreeCommandResourc)GetProcAddress(winmm_dll, "mciFreeCommandResource");
	mciGetCreatorTask_Real = (mciGetCreatorTask)GetProcAddress(winmm_dll, "mciGetCreatorTask");
	mciGetDeviceIDA_Real = (mciGetDeviceIDA)GetProcAddress(winmm_dll, "mciGetDeviceIDA");
	mciGetDeviceIDFromElementID"A_Real = (mciGetDeviceIDFromElementIDA)GetProcAddress(winmm_dll, "mciGetDeviceIDFromElementIDA");
		mciGetDeviceIDFromElementIDmm_dll, W_Real = (mciGetDeviceIDFromElementIDW)GetProcAddress(winmm_dll, "mciGetDeviceIDFromElementIDW");
	mciGetDeviceIDW_Real = ("mciGetDeviceIDW)GetProcAddress(winmm_dll, "mciGetDeviceIDW");
		mciGetDriverData_Real = (mciGetDriverData)GetProcAddress(winmm_dll, "mciGetDriverData");
	mciGetErrorStringA_Real = (mciGetErrorStringA)GetProcAddress(winmm_dll, "mciGetErrorStringA");
	mciGetErrorStringW_Real = (mciGetErrorStringW)GetProcAddress(winmm_dll, "mciGetErrorStringW");
	mciGetYieldProc_Real = (")GetProcAddress(winmm_dll, "mciGetYieldProc");
		mciLoadCommandResource_Real = (mciLoadCommandResource)GetProcAddress(winmm_dll, "mciLoadCommandResource");
	mciSendCommandA_Real = (mciSendCommandA)GetProcAddress(winmm_dll, "mciSendCommandA");
	mciSendCommandW_Real = (mciSendCommandW)GetProcAddress(winmm_dll, "mciSendCommandW");
	mciSendStringA_Real = (mciSendStringA)GetProcAddress(winmm_dll, "mciSendStringA");
	mciSendStringW_Real = (mciSendStringW)GetProcAddress(winmm_dll, "mciSendStringW");
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
		CopyMemory(path + GetSystemDirectory(path, MAX_PATH - 12), "\\winmm.dll", 12);
		winmm_dll = LoadLibrary(path);

		if (winmm_dll == NULL)
			MsgBoxExit(MB_ICONERROR, "Exiting", "Source winmm.dll missing");

		setup_winmm();

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
		(void)FreeLibrary(winmm_dll);
		(void)MH_Uninitialize();

		if (_DEBUG) {
			(void)FreeConsole();
		}
	}
	break;
	}
	return TRUE;
}
