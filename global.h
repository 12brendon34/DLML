#pragma once
#include "common.h"

class global {
public:
    bool Running = true;
    bool MinHookInitialized = false;
    fs::path WorkingDir;
};

inline global globals;