#pragma once
#include "memory.hpp"

struct Timer {
    Memory* memory;
    int cycles_threshold{};
    int curr_cycles{};
    bool tima_enable{};

    Timer(Memory* mem);
    void update_timers();
    void tick();
};