#include "Utils.h"

auto Utils::MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...) -> void {

	char ach[512];
	va_list args;
	va_start(args, sz);
	(void)wvsprintf(ach, sz, args);
	(void)MessageBox(nullptr, ach, lpCaption, nType);
	ExitProcess(0);
}

auto Utils::str_tolower(std::string s) -> std::string
{
	std::ranges::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return s;
}

auto Utils::GetModuleHandleSimple(LPCSTR lpModuleName) -> HMODULE {

	HMODULE Handle = GetModuleHandle(lpModuleName);
	if (Handle)
		(void)dbgprintf("Found %s BaseAddress at: %p\n", lpModuleName, Handle);
	else
		Utils::MsgBoxExit(MB_ICONERROR, "Exiting", "Unable to create handle for %s", lpModuleName);

	return Handle;
}

auto Utils::GetProcAddressSimple(HMODULE hModule, LPCSTR lpProcName) -> FARPROC {

#pragma warning(suppress : 6387)
	FARPROC Address = GetProcAddress(hModule, lpProcName);
	if (Address)
		(void)dbgprintf("Loaded Libary %s at: %p\n", lpProcName, Address);
	else
		Utils::MsgBoxExit(MB_ICONERROR, "Exiting", "Failed To Get Address of %s", lpProcName);

	return Address;
}

auto Utils::HookFunction(LPVOID target, LPVOID destination, LPVOID* original, const char* name, bool enable) -> void {

	MH_STATUS status = MH_CreateHook(target, destination, original);
	std::string statusCode = MH_StatusToString(status);

	if (status == MH_OK) {
		if (name) 
			(void)dbgprintf("Hooked \"%s\" at: %p -> %p\n", name, target, destination);
		else
			(void)dbgprintf("Hooked %p -> %p\n", target, destination);
	}
	else {
		if (name) 
			Utils::MsgBoxExit(MB_ICONERROR, "Exiting", "Failed to hook \"%s\" at: %p, Status Code: %s",name, target, statusCode.c_str());
		else
			Utils::MsgBoxExit(MB_ICONERROR, "Exiting", "Failed to hook %p, Status Code: %s", target, statusCode.c_str());
	}
	if (enable)
		(void)MH_EnableHook(target);

}