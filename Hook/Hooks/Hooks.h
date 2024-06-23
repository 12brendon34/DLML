#pragma once
#include "../Hook.h"
#include "../../Menu.h"

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

class CLogV : public Hook {
public:
	explicit CLogV(DLML* dlml) : Hook(dlml) {}
	auto init(void) -> Status override;
};

class GetCategoryLevel : public Hook {
public:
	explicit GetCategoryLevel(DLML* dlml) : Hook(dlml) {}
	auto init(void) -> Status override;
};

class Log_Settings_Instance : public Hook {
public:
	explicit Log_Settings_Instance(DLML* dlml) : Hook(dlml) {}
	auto init(void) -> Status override;
};
