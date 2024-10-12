#pragma once
#include "constants.hpp"

struct Joypad {
    u8 input_buffer{}, intrF{}, joyp{};
    u8 read();
    void write(u8 data);
};