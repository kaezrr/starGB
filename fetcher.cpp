#include "fetcher.hpp"

Sprite::Sprite(u16 at, Memory* mem) {
    if (at < OAM_S || at > OAM_E) return;
    posY            = mem->oam[at + 0 - OAM_S];
    posX            = mem->oam[at + 1 - OAM_S];
    tile_id         = mem->oam[at + 2 - OAM_S];
    u8 flag         = mem->oam[at + 3 - OAM_S];
    obj_priority    = (flag & 0x80) >> 7;
    flipY           = (flag & 0x40) >> 6;
    flipX           = (flag & 0x20) >> 5;
    palette         = (flag & 0x10) >> 4;
    used = false;
}

Fetcher::Fetcher(Memory* mem) : memory{ mem } {
    sprite_buffer.reserve(10); new_frame();
}

void Fetcher::new_line() {
    sprite_buffer.clear();
    tile_index = 0; x_pos = -8;
    queue_bg = 0; bg_data = 0; bg_count = 0;
    queue_sp = 0; sp_data = 0; sp_count = 0;
    delay = true; fetch_window = false;
}

void Fetcher::check_window() {
    if ((x_pos >= wx() - 7) && wy_cond && (lcdc() & 0x20)) {
        if (fetch_window) return;
        fetch_window = true;
        bg_state = Fetcher_State::READ_TILE_ID;
        queue_bg = 0; tile_index = 0;
    } else {
        fetch_window = false;
    }
}

void Fetcher::new_frame() {
    new_line(); wy_cond = false;
    window_line_counter = 0;
}

void Fetcher::inc_windowline() {
    window_line_counter += increment_window;
    increment_window = false;
}

bool Fetcher::check_sprite() {
    if (!(lcdc() & 2)) return false;
    for (auto& e : sprite_buffer) {
        if (e.used || e.posX > x_pos + 8) continue;
        bg_state = Fetcher_State::PAUSED; 
        return true;
    }
    return false;
}

void Fetcher::sp_fetch_tile_no() {
    for (auto& sp : sprite_buffer) {
        if (sp.used || sp.posX > x_pos + 8) continue;
        sp_tile_no = sp.tile_id; flipx = sp.flipX; flipy = sp.flipY;
        u16 attr = sp.obj_priority | (sp.palette << 1);
        sp_data = attr * 0x11111111; sp.used = true;
        if (lcdc() & 4) {
            sp_tile_no &= ~1;
            sp_tile_no += (ly() + 16 - sp.posY >= 8);
            sp_tile_no ^= sp.flipY;
        }
        break;
    }
}

void Fetcher::sp_fetch_tile_data(bool state) {
    u16 addr = 0x8000 + (sp_tile_no * 16);
    u16 tile_line = (ly() + scy()) % 8;
    if (flipy) tile_line = 7 - tile_line;
    u8 data = vram(addr + (tile_line * 2) + state);
    for (u8 i = 0; i < 8; ++i) {
        int shift = flipx ? 7 - i : i;
        sp_data |= ((data & (1 << shift)) >> shift) << (i * 4 + 2 + state);
    }
}

void Fetcher::sp_push_to_fifo() {
    u32 mask = ~queue_sp & (~queue_sp << 1) & 0x88888888;
    mask = (mask << 1) - (mask >> 3);
    queue_sp |= sp_data & mask;
    sp_count = 8; sp_data = 0;
    flipx = flipy = false;
}


void Fetcher::bg_fetch_tile_no() {
    u16 tile_x{}, tile_y{}, tile_map{};
    if (fetch_window && (lcdc() & 0x20)) {
        increment_window = true;
        tile_y = window_line_counter / 8;
        tile_x = (tile_index & 0xFF) / 8;
        tile_map = (lcdc() & 0x40) ? 0x9C00 : 0x9800;
    } else {
        tile_y = ((scy() + ly()) & 0xFF) / 8;
        tile_x = ((scx() + tile_index) & 0xFF) / 8;
        tile_map = (lcdc() & 0x08) ? 0x9C00 : 0x9800;
    }
    bg_tile_no = vram(tile_map + (32 * tile_y) + tile_x);
}

void Fetcher::bg_fetch_tile_data(bool state) {
    u16 addr{}, offs{};
    if(lcdc() & 0x10) addr = 0x8000 + (bg_tile_no * 16);
    else addr = 0x9000 + (static_cast<s8>(bg_tile_no) * 16);

    if (fetch_window && (lcdc() & 0x20)) offs = (window_line_counter % 8) * 2;
    else offs = ((ly() + scy()) % 8) * 2;

    u8 data = vram(addr + offs + state);
    for (u8 i = 0; i < 8; ++i)
        bg_data |= ((data & (1 << i)) >> i) << (i * 2 + state);
}

bool Fetcher::bg_push_to_fifo() {
    if (bg_count) return false;
    queue_bg = bg_data;
    bg_count = 8; bg_data = 0;
    return true;
}

void Fetcher::sp_tick() {
    switch (sp_state) {
    case Fetcher_State::SP_READ_TILE_ID:
        sp_fetch_tile_no();
        sp_state = Fetcher_State::SP_READ_TILE_DATA0;
        break;
    case Fetcher_State::SP_READ_TILE_DATA0:
        sp_fetch_tile_data(false);
        sp_state = Fetcher_State::SP_READ_TILE_DATA1;
        break;
    case Fetcher_State::SP_READ_TILE_DATA1:
        sp_fetch_tile_data(true);
        sp_state = Fetcher_State::SP_PUSH_TO_FIFO;
        break;
    case Fetcher_State::SP_PUSH_TO_FIFO:
        sp_push_to_fifo();
        sp_state = Fetcher_State::SP_READ_TILE_ID;
        bg_state = Fetcher_State::READ_TILE_ID;
        break;
    }
}

bool Fetcher::bg_tick() {
    switch (bg_state) {
    case Fetcher_State::READ_TILE_ID: 
        bg_fetch_tile_no();
        bg_state = Fetcher_State::READ_TILE_DATA0;
        break;

    case Fetcher_State::READ_TILE_DATA0: 
        bg_fetch_tile_data(false);
        bg_state = Fetcher_State::READ_TILE_DATA1;
        break;

    case Fetcher_State::READ_TILE_DATA1:
        bg_fetch_tile_data(true);
        bg_state = Fetcher_State::PUSH_TO_FIFO;
        break;

    case Fetcher_State::PUSH_TO_FIFO:
        if (bg_push_to_fifo()) {
            bg_state = Fetcher_State::READ_TILE_ID;
            tile_index += (x_pos < 0) ? 0 : 8;
            if (check_sprite()) return false;
        }
        break;

    case Fetcher_State::PAUSED:
        sp_tick();
        return false;
    }
    return true;
}
