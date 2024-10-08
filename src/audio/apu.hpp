#pragma once
#include "memory.hpp"

struct Channel1 {};

struct APU {
    bool enabled{false};
    Memory* memory{nullptr};

    void tick();
    Channel1 ch1;
};
