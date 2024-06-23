#pragma once
#include "../Hook.h"

class Add_Source : public Hook {
public:
	explicit Add_Source(DLML* dlml) : Hook(dlml) {}
	auto init(void) -> Status override;
};

class InitalizeGameScript : public Hook {
public:
	explicit InitalizeGameScript(DLML* dlml) : Hook(dlml) {}
	auto init(void) -> Status override;
};
