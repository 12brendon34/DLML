#pragma once
#include "Loader.h"

class Hook;

class DLML {
public:
	HMODULE FilesystemDll;
	HMODULE EngineDll;
public:
	DLML();
public:
	std::vector<Hook*> hooks;
public:
	auto initMinHook(void) const ->Status;
	auto initHooks(void) -> Status;
	auto HookRenderer(void) -> void;
};