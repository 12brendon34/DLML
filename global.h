#pragma once
#include "common.h"

class global {
public:
    bool Running = true;
    bool MinHookInitialized = false;
};

inline global globals;