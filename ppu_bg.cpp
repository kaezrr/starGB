#include "ppu.hpp"
#include <iostream>

void PPU::bg_fetch_tile_no() {
    u16 ly = memory->read(LY);
    u8 scy = memory->read(SCY);
    u8 scx = memory->read(SCX);
    u16 tile_x = ((scx + x_pos) & 0xFF) / 8;
    u16 tile_y = ((scy + ly) & 0xFF) / 8;
    bg_tile_no = memory->read(TILE_MAP1 + (32 * tile_y) + tile_x);
}

void PPU::bg_fetch_tile_data(bool state) {
    u16 ly = memory->read(LY);
    u8 scy = memory->read(SCY);
    u16 addr = 0x8000 + (bg_tile_no * 16);
    u16 offs = ((ly + scy) % 8) * 2;
    if (!state) {
        u8 lsb = memory->read(addr + offs);
        for (u8 i = 0; i < 8; ++i)
            bg_data |= (lsb & (1 << i)) << i;
    } else {
        u8 msb = memory->read(addr + offs + 1);
        for (u8 i = 0; i < 8; ++i)
            bg_data |= (msb & (1 << i)) << (i + 1);
    }
}

bool PPU::bg_push_to_fifo() {
    if (bg_count) return false;
    queue_bg = bg_data; bg_count = 8; bg_data = 0;
    return true;
}
