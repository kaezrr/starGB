#include "joypad.hpp"

u8 Joypad::read() {
    u8 reg = joyp & 0xF0;
    u8 select = (~(input_buffer >> 4)) & 0xF;
    u8 dpad = (~input_buffer) & 0xF;
    switch ((reg >> 4) & 3) {
    case 0:
        return reg | (select & dpad);
    case 1:
        return reg | (select);
    case 2:
        return reg | (dpad);
    case 3:
        return reg | 0xF;
    }
    return 0xFF;
}

void Joypad::write(u8 data) { joyp = data; }