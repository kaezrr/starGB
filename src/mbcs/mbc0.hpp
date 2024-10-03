#pragma once

#include <vector>
#include <array>
#include <string>
#include "constants.hpp"

using std::vector;
using std::array;
using std::string;

struct MBC0 {
    bool save_flag{};
    string save_path{};
    u16 rom_num{}, ram_num{};

    MBC0() = default;
    MBC0(const string& path); 

    array<u8, 0x8000> rom_bank;
    array<u8, 0x2000> ram_bank;

    void save_ram();
    u8 read_rom(u16 at) const;
    u8 read_ram(u16 at) const;
    void write_rom(u16 at, u8 data);
    void write_ram(u16 at, u8 data);
};