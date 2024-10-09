#pragma once

#include "constants.hpp"
#include "mbc.hpp"
#include "timer.hpp"
#include <string>
#include <vector>
#include <memory>

using std::vector;
using std::array;
using std::string;

struct Memory {
    std::unique_ptr<MBC> mbc{nullptr};
    Timer* timer{nullptr};

    vector<u8> boot_rom     = vector<u8>(0x0100);
    vector<u8> vram         = vector<u8>(0x2000);
    vector<u8> wram         = vector<u8>(0x2000);
    vector<u8> oam          = vector<u8>(0x00A0);
    vector<u8> io_reg       = vector<u8>(0x0080);
    vector<u8> hram         = vector<u8>(0x007F);

    bool execute_boot{};
    u8 input_buffer{}, ie_reg{};

    Memory(Timer* timer_ptr) : timer{timer_ptr} {}

    u8 read(u16 at) const;
    u8 read_IO(u16 at) const;
    void write(u16 at, u8 data);
    void write_IO(u16 at, u8 data);
    void initiate_dma_transfer(u8 data);

    void load_game(const string& path);
    void load_boot(const string& path);
};

// Update original with data, while retaining the masked bits of the original
inline void update_read_only(u8& original, u8 data, u8 mask) {
    original = (original & mask) | (data & ~mask);
}