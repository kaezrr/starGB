#pragma once

#include<algorithm>
#include<vector>
#include<array>
#include "constants.hpp"

using std::vector;
using std::array;

struct Memory {
#ifndef TEST
    vector<u8> rom_banks    = vector<u8>(0x8000);
    vector<u8> vram         = vector<u8>(0x2000);
    vector<u8> exram        = vector<u8>(0x2000);
    vector<u8> wram         = vector<u8>(0x2000);
    vector<u8> oam          = vector<u8>(0x00A0);
    vector<u8> io_reg       = vector<u8>(0x0080);
    vector<u8> hram         = vector<u8>(0x007F);

    u8 ie_reg{};
    bool oam_lock{};
#else 
    vector<u8> test_memory  = vector<u8>(0x10000);
#endif

    u8 read(u16 at) const;
    void write(u16 at, u8 data);
    void reset();
};