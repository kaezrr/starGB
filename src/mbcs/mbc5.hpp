#pragma once

#include<vector>
#include<array>
#include<string>
#include "constants.hpp"

using std::vector;
using std::array;
using std::string;

struct MBC5 {
    bool save_flag{}, exram_enable{};
    u16 rom_num{}, ram_num{};

    MBC5() = default;
    MBC5(const string& path); 

    string save_path{};
    vector<u8> rom_banks;
    vector<u8> ram_banks;

    void save_ram();
    u8 read_rom(u16 at) const;
    u8 read_ram(u16 at) const;
    void write_rom(u16 at, u8 data);
    void write_ram(u16 at, u8 data);
};