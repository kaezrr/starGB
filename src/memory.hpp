#pragma once

#include "constants.hpp"
#include "joypad.hpp"
#include "mbc.hpp"
#include "ppu.hpp"
#include "timer.hpp"
#include <memory>
#include <string>
#include <vector>

using std::array;
using std::string;
using std::vector;

struct Memory {
    std::unique_ptr<MBC> mbc{nullptr};
    PPU* ppu{nullptr};
    Timer* timer{nullptr};
    Joypad* joypad{nullptr};

    vector<u8> boot_rom = vector<u8>(0x0100);
    vector<u8> wram = vector<u8>(0x2000);
    vector<u8> io_reg = vector<u8>(0x0080);
    vector<u8> hram = vector<u8>(0x007F);

    bool execute_boot{};
    u8 ie_reg{}, serial_intrF{};

    Memory(Timer* tptr, PPU* pptr, Joypad* jptr)
        : ppu{pptr}, timer{tptr}, joypad{jptr} {}

    u8 read(u16 at) const;
    u8 read_IO(u16 at) const;
    void write(u16 at, u8 data);
    void write_IO(u16 at, u8 data);
    void initiate_dma_transfer(u8 data);

    void load_game(const string& path);
    void load_boot();

    u8 get_intrF() const;
    void set_intrF(u8 data);
};

// Update original with data, while retaining the masked bits of the original
inline void update_read_only(u8& original, u8 data, u8 mask) {
    original = (original & mask) | (data & ~mask);
}
