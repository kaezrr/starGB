#pragma once

#include <vector>
#include <array>
#include <string>
#include "constants.hpp"

using std::vector;
using std::array;
using std::string;

struct MBC {
    virtual void save_ram() = 0;
    virtual u8 read_rom(u16 at) const = 0;
    virtual u8 read_ram(u16 at) const = 0;
    virtual void write_rom(u16 at, u8 data) = 0;
    virtual void write_ram(u16 at, u8 data) = 0;
    virtual ~MBC() = default;
};

struct MBC0 : public MBC {
    bool save_flag{};
    string save_path{};
    u16 rom_num{}, ram_num{};

    MBC0() = default;
    MBC0(const string& path); 

    array<u8, 0x8000> rom_bank;
    array<u8, 0x2000> ram_bank;

    void save_ram() override;
    u8 read_rom(u16 at) const override;
    u8 read_ram(u16 at) const override;
    void write_rom(u16 at, u8 data) override;
    void write_ram(u16 at, u8 data) override;
};

struct MBC1 : public MBC {
    u16 rom_num{1}, ram_num{};
    bool save_flag{}, exram_enable{}, mode_flag{};

    MBC1() = default;
    MBC1(const string& path); 

    string save_path{};
    vector<u8> rom_banks;
    vector<u8> ram_banks;

    void save_ram() override;
    u8 read_rom(u16 at) const override;
    u8 read_ram(u16 at) const override;
    void write_rom(u16 at, u8 data) override;
    void write_ram(u16 at, u8 data) override;
};

struct MBC3 : public MBC {
    u8 rom_num{1}, ram_num{};
    bool save_flag{}, exram_enable{};

    MBC3() = default;
    MBC3(const string& path); 

    string save_path{};
    vector<u8> rom_banks;
    vector<u8> ram_banks;

    void save_ram() override;
    u8 read_rom(u16 at) const override;
    u8 read_ram(u16 at) const override;
    void write_rom(u16 at, u8 data) override;
    void write_ram(u16 at, u8 data) override;
};

struct MBC5 : public MBC {
    u16 rom_num{1}, ram_num{};
    bool save_flag{}, exram_enable{}, rumble{};

    MBC5() = default;
    MBC5(const string& path); 

    string save_path{};
    vector<u8> rom_banks;
    vector<u8> ram_banks;

    void save_ram() override;
    u8 read_rom(u16 at) const override;
    u8 read_ram(u16 at) const override;
    void write_rom(u16 at, u8 data) override;
    void write_ram(u16 at, u8 data) override;
};