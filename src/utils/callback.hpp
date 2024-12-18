#pragma once
#include "constants.hpp"

using fn_type = void (*)(void*, u8*);

struct CallBack {
    fn_type fn = nullptr;
    void* instance = nullptr;

    CallBack(void* ins, fn_type f) : fn{f}, instance{ins} {}

    void call(u8* buffer) { return fn(instance, buffer); }
};
