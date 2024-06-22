#pragma once

#include "../Utils/Utils.h"
#include "../DLML.h"
class DLML;

class Hook {
public:
	DLML* pDLML;
public:
	Hook(DLML*);
public:
	virtual auto init(void) -> Status { 
		return Status(MethodStatus::Error, "Not Overridden"); 
	};
};