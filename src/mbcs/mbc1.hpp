#pragma once

#include<vector>
#include<string>
#include "constants.hpp"

using std::vector;
using std::string;

struct MBC1 {
    u16 rom_num{1}, ram_num{};
    bool save_flag{}, exram_enable{}, rumble{};

    MBC1() = default;
    MBC1(const string& path); 

    string save_path{};
    vector<u8> rom_banks;
    vector<u8> ram_banks;

    void save_ram();
    u8 read_rom(u16 at) const;
    u8 read_ram(u16 at) const;
    void write_rom(u16 at, u8 data);
    void write_ram(u16 at, u8 data);
};