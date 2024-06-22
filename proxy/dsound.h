#pragma once
#include "../Utils/Utils.h"
#include <dsound.h>

class Dsound {
public:
	static auto init(void) -> HMODULE;
};