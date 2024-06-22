#pragma once

#include "../../Hook.h"

class Add_Source : public Hook {
public:
	explicit Add_Source(DLML* dlml) : Hook(dlml) {}
	virtual ~Add_Source() = default;
	auto init(void) -> Status override;
};
