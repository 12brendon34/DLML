#pragma once
#include "common.h"

class global {
public:
    bool DyingLight2 = false;
    bool Running = true;
    bool MinHookInitialized = false;
    std::filesystem::path WorkingDir;
};

inline global globals;