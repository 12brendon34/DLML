#include "Hook.h"
#include "../DLML.h"

Hook::Hook(DLML* dlml) {

	pDLML = dlml;
	pDLML->hooks.push_back(this);

};