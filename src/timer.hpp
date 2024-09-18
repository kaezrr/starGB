#pragma once
#include "constants.hpp"
#include "memory.hpp"

struct Timer {
    Memory* memory;

    Timer(Memory* mem);
    void tick();
    void req_timer_intr();
};