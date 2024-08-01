#pragma once

#include<algorithm>
#include<vector>
#include<array>
#include "constants.hpp"

using std::vector;
using std::array;

struct Memory {
    vector<u8> boot_rom     = vector<u8>(0x100);
    vector<u8> rom_banks    = vector<u8>(0x8000);
    vector<u8> vram         = vector<u8>(0x2000);
    vector<u8> exram        = vector<u8>(0x2000);
    vector<u8> wram         = vector<u8>(0x2000);
    vector<u8> oam          = vector<u8>(0x00A0);
    vector<u8> io_reg       = vector<u8>(0x0080);
    vector<u8> hram         = vector<u8>(0x007F);

    u8 ie_reg{};
    u16 sys_clock{};
    bool oam_lock{}, execute_boot{}; 
    bool tima_watch{}, tima_write{};

    u8 read(u16 at) const;
    void write(u16 at, u8 data);
    void reset();
};