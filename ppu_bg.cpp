#include "ppu.hpp"

void PPU::push_to_display() {
    if (!bg_count) return;
    if (scx_discard) {
        queue_bg <<= 2 * (scx() % 8);
        scx_discard = false;
    }
    u16 bg_pixel = (lcdc() & 1) ? (queue_bg >> 14) : 0; // pop 2 bits from the fifo
    queue_bg <<= 2; --bg_count;
    u16 select = bg_pixel, palette = bgp(); // select appropriate color from palette

    if (sp_count) {
        u16 sp_pixel = (queue_sp >> 28);
        queue_sp <<= 4; --sp_count;
        if ((sp_pixel >> 2) && !((sp_pixel & 1) && bg_pixel)) {
            select = sp_pixel >> 2;
            palette = (sp_pixel & 2) ? obp0() : obp1();
        }
    }

    u32 col = colors[(palette >> (select * 2)) & 0x3];
    display[pixel_pos(ly(), x_pos++)] = col;

    if (display_window()) {
        if (fetch_window) return;
        fetch_window = true;
        fstate = Fetcher_State::READ_TILE_ID;
        queue_bg = 0; tile_index = 0;
    } else {
        fetch_window = false;
    }
}

void PPU::bg_fetch_tile_no() {
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

void PPU::bg_fetch_tile_data(bool state) {
    u16 addr{}, offs{};
    if(lcdc() & 0x10) addr = 0x8000 + (bg_tile_no * 16);
    else addr = 0x9000 + (static_cast<s8>(bg_tile_no) * 16);

    if (fetch_window && (lcdc() & 0x20)) offs = (window_line_counter % 8) * 2;
    else offs = ((ly() + scy()) % 8) * 2;

    if (!state) {
        u8 lsb = vram(addr + offs);
        for (u8 i = 0; i < 8; ++i)
            bg_data |= (lsb & (1 << i)) << i;
    } else {
        u8 msb = vram(addr + offs + 1);
        for (u8 i = 0; i < 8; ++i)
            bg_data |= (msb & (1 << i)) << (i + 1);
    }
}

bool PPU::bg_push_to_fifo() {
    if (bg_count) return false;
    queue_bg = bg_data;
    bg_count = 8; bg_data = 0;
    return true;
}


