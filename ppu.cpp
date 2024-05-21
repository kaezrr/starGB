#include "ppu.hpp"

Sprite::Sprite(u16 at, Memory& mem) {
    if (at < OAM_S || at > OAM_E) return;
    posY            = mem.read(at + 0);
    posX            = mem.read(at + 1);
    tile_id         = mem.read(at + 2);
    u8 flag         = mem.read(at + 3);
    obj_priority    = flag & 0x80;
    flipY           = flag & 0x40;
    flipX           = flag & 0x20;
    palette         = flag & 0x10;
}


PPU::PPU(Memory* mem_ptr) : memory{mem_ptr} {
    sprite_buffer.reserve(10);
}

void PPU::tick() {
    switch (mode) {
    case Mode::HBLANK: hblank();
        break;
    case Mode::VBLANK: vblank();
        break;
    case Mode::OAM_SCAN: // 2 oam_scans take 4 t-cycles
        oam_scan(); oam_scan();
        break;
    case Mode::DRAWING:
        break;
    }
}

void PPU::add_sprite(u16 at) {
    dots += 2;
    if (sprite_buffer.size() >= 10) return;
    int ly_check = memory->read(LY) + 16;
    int s_height = (memory->read(LCDC) & 0x04) ? 16 : 8;
    int posy = memory->read(at + 0);
    int posx = memory->read(at + 1);
    if (posx >= 0 && ly_check >= posy && ly_check < posy + s_height)
        sprite_buffer.emplace_back(at, *memory);
}

void PPU::oam_scan() {
    add_sprite(curr_sprite_location);
    curr_sprite_location += 4;
    if (dots < 80) return;

    curr_sprite_location = OAM_S;
    x_pos = 0;
    mode = Mode::DRAWING;
}

void PPU::hblank() {
    dots += 4;
    if (dots < 456) return;
    dots = 0;
    mode = (memory->read(LY) < 144) ? Mode::OAM_SCAN : Mode::VBLANK;
    if (mode == Mode::VBLANK) window_line_counter = 0;
}

void PPU::vblank() {
    dots += 4;
    if (dots < 456) return;
    dots = 0;
    mode = (memory->read(LY) < 154) ? Mode::VBLANK : Mode::OAM_SCAN;
    if (mode == Mode::VBLANK) window_line_counter = 0;
}

void PPU::fetch_tile_no() {
    auto lcdc = memory->read(LCDC);

}

void PPU::fetch_tile_data(bool state) {

}

void PPU::push_to_fifo() {

}