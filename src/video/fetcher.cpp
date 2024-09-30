#include "fetcher.hpp"
#include <cmath>

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
    tile_index = x_pos = -8;
    queue_bg = bg_data = bg_count = 0;
    queue_sp = sp_data = sp_count = 0;
    fetch_window = false; delay = true;
}

void Fetcher::check_window() {
    if ((x_pos >= wx() - 7) && wy_cond && (lcdc() & 0x20)) {
        tile_index = 0;
        fetch_window = true;
        queue_bg = bg_count = 0; 
        bg_state = Fetcher_State::READ_TILE_ID;
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
        if (e.posX > x_pos + 8) continue;
        return true;
    }
    return false;
}

void Fetcher::sp_fetch_tile_no() {
    for (auto& sp : sprite_buffer) {
        if (sp.posX > x_pos + 8) continue;
        u16 attr = sp.obj_priority | (sp.palette << 1);
        sp_data = attr * 0x11111111;
        curr_sp = sp; sp.posX = 0xFF; break;
    }
}

void Fetcher::sp_fetch_tile_data(bool state) { // state = HIGH/LOW
    u16 sp_tile_no = curr_sp.tile_id;
    // check for sprite size (8 or 16)
    if (lcdc() & 4) {
        sp_tile_no &= ~1;
        sp_tile_no += (ly() + 16 - curr_sp.posY >= 8);
        sp_tile_no ^= curr_sp.flipY;
    }
    u16 addr = 0x8000 + (sp_tile_no * 16);
    u16 tile_line = (ly() + 16 - curr_sp.posY) % 8;
    if (curr_sp.flipY) tile_line = 7 - tile_line;
    // fill sprite fifo with data
    u8 data = vram(addr + (tile_line * 2) + state);
    for (u8 i = 0; i < 8; ++i) {
        int shift = curr_sp.flipX ? 7 - i : i;
        sp_data |= ((data & (1 << shift)) >> shift) << (i * 4 + 2 + state);
    }
}

void Fetcher::sp_push_to_fifo() {
    for (int i = 0; i < 8; ++i) {
        u32 mask = 0xF << (i * 4);
        u8 fifo_px = (u8)((queue_sp & mask) >> (i * 4));
        u8 data_px = (u8)((sp_data & mask) >> (i * 4));
        u8 dcol = (data_px & 12) >> 2;
        u8 fcol = (fifo_px & 12) >> 2;
        u8 dpr = (data_px & 1);
        u8 fpr = (fifo_px & 1);

        if ((dcol && !fcol) || (dpr > fpr)) {
            queue_sp &= ~mask;
            queue_sp |= sp_data & mask;
        }
    }
    sp_count = 8; sp_data = 0;
}


void Fetcher::bg_fetch_tile_no() {
    u16 tile_x{}, tile_y{}, tile_map{};
    if (fetch_window && (lcdc() & 0x20)) {
        increment_window = true;
        tile_y = window_line_counter / 8;
        tile_x = (std::max(tile_index + 7, 0) & 0xFF) / 8;
        tile_map = (lcdc() & 0x40) ? 0x9C00 : 0x9800;
    } else {
        tile_y = ((scy() + ly()) & 0xFF) / 8;
        tile_x = ((scx() + std::max(tile_index + 7, 0)) & 0xFF) / 8;
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
    if(!state) bg_data = 0;
    for (u8 i = 0; i < 8; ++i)
        bg_data |= ((data & (1 << i)) >> i) << (i * 2 + state);
}

bool Fetcher::bg_push_to_fifo() {
    if (bg_count) return false;
    queue_bg = bg_data;
    bg_count = 8;
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
    default: break;
    }
}

bool Fetcher::bg_tick() {
    if (bg_state != Fetcher_State::PAUSED && check_sprite()) {
        bg_state = Fetcher_State::PAUSED;
        return false;
    }

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
        if (bg_push_to_fifo()) 
            bg_state = Fetcher_State::READ_TILE_ID;
        break;

    case Fetcher_State::PAUSED:
        sp_tick();
        return false;
    
    default: break;
    }
    return true;
}
