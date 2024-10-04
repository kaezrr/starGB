#pragma once

#include "constants.hpp"
#include "mbc.hpp"
#include <string>
#include <vector>
#include <memory>

using std::vector;
using std::array;
using std::string;

struct Memory {
    std::unique_ptr<MBC> mbc{nullptr};

    vector<u8> boot_rom     = vector<u8>(0x0100);
    vector<u8> vram         = vector<u8>(0x2000);
    vector<u8> wram         = vector<u8>(0x2000);
    vector<u8> oam          = vector<u8>(0x00A0);
    vector<u8> io_reg       = vector<u8>(0x0080);
    vector<u8> hram         = vector<u8>(0x007F);
    u8 input_buffer{};

    u8 ie_reg{};
    bool execute_boot{}, tima_write{};

    // Timer stuff
    u16 sys_clock{ 0 }, last_edge{ 0 };
    int cycles_til_tima_irq{ 0 };
    bool tima_reload_cycle{ false };

    u8 read(u16 at) const;
    u8 read_IO(u16 at) const;
    void write(u16 at, u8 data);
    void write_IO(u16 at, u8 data);
    void initiate_dma_transfer(u8 data);

    void sys_clock_change(u16 new_value);
    void detect_edge(u16 before, u16 after);
    static void update_read_only(u8& original, u8 data, u8 mask);

    void load_game(const string& path);
    void load_boot(const string& path);
};