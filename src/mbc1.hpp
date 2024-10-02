#pragma once

#include<vector>
#include<array>

#include "constants.hpp"

using std::vector;
using std::array;

struct MBC1 {
    bool mode_flag{}, save_flag{};
    u8 rom_bank_no{}, ram_bank_no{};

    MBC1(u8 rom_size, u8 ram_size, bool save); 

    vector<array<u8, 0x4000>> rom_banks;
    vector<array<u8, 0x2000>> ram_banks;
};