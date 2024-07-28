#include "ppu.hpp"

Sprite::Sprite(u16 at, Memory* mem) {
    if (at < OAM_S || at > OAM_E) return;
    posY            = mem->read(at + 0);
    posX            = mem->read(at + 1);
    tile_id         = mem->read(at + 2);
    u8 flag         = mem->read(at + 3);
    obj_priority    = flag & 0x80;
    flipY           = flag & 0x40;
    flipX           = flag & 0x20;
    palette         = flag & 0x10;
}

void PPU::add_sprite(u16 at) {
    if (sprite_buffer.size() >= 10) return;
    int ly_check = memory->read(LY) + 16;
    int s_height = (memory->read(LCDC) & 0x04) ? 16 : 8;
    int posy = memory->read(at + 0);
    int posx = memory->read(at + 1);

    // Add sprite if the below conditions apply
    if (posx > 0 && ly_check >= posy && ly_check < posy + s_height)
        sprite_buffer.emplace_back(at, memory);
}

bool PPU::sp_fetch_tile_no() {
    bool flag = false;
    for (const auto& sp : sprite_buffer) {
        if (sp.posX < x_pos + 8) continue;
        flag = true;
        sp_tile_no = sp.tile_id;
        auto attr = sp.obj_priority + sp.palette;
        sp_data = attr * 0x11111111;
    }
    return flag;
}

void PPU::sp_fetch_tile_data(bool state) {
    u8  scy  = memory->read(SCY);
    u8  ly   = memory->read(LY);
    u8  addr = (u8)(0x8000 + bg_tile_no * 16);
    u16 offs = display_window() ? 2 * (window_line_counter % 8) : 2 * ((ly + scy) % 8);
    if (!state) {
        u8 lsb = memory->read(addr + offs);
        for (u8 i = 0; i < 8; ++i)
            sp_data |= (lsb & (1 << i)) << ((i * 4) + 2);
    } else {
        u8 msb = memory->read(addr + offs + 1);
        for (u8 i = 1; i < 8; ++i)
            sp_data |= (msb & (1 << i)) << ((i * 4) + 3);
    }
}

void PPU::sp_push_to_fifo() {
    u32 mask = ~queue_sp & (~queue_sp << 1) & 0x88888888;
    mask = (mask << 1) - (mask >> 3);
    queue_sp |= sp_data & mask;
}

