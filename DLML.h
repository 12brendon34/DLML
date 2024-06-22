#pragma once
#include "Utils/Utils.h"

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
	auto initMinHook(void) -> Status;
	auto initHooks(void) -> Status;
};