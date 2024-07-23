#pragma once
#include "constants.hpp"
#include "memory.hpp"

struct Timer {
    Memory* memory;
    u8 overflow_cycles{};
    bool prev_result{}, pending_overflow{};

    Timer(Memory* mem);
    void tick();
    void t_tick();
    void overflow();
};