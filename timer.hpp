#pragma once
#include "constants.hpp"
#include "memory.hpp"

struct Timer {
    Memory* memory;
    u8 overflow_cycles{};
    bool prev_edge{}, pending_overflow{};

    Timer(Memory* mem);
    void tick();
    void overflow();
    void req_timer_intr();
};