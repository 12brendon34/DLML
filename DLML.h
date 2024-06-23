#pragma once
#include "Utils/Utils.h"
#include "Hook/Hooks/Hooks.h"
#include "Loader.h"
#include "kiero/kiero.h"
#include "impl/d3d11_impl.h"

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