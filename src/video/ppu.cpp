#include "ppu.hpp"
#include "constants.hpp"
#include "memory.hpp"
#include <spdlog/spdlog.h>

void PPU::new_frame() {
    fetcher.new_frame();
    increment_ly();
    renderer.call(display.data());
    curr_sprite_location = OAM_S;
    new_line();
    dots = 0;
}

void PPU::new_line() {
    scx_discard = true;
    fetcher.new_line();
}

void PPU::increment_ly() {
    fetcher.ly = fetcher.ly >= 153 ? 0 : fetcher.ly + 1;
    fetcher.inc_windowline();
    if (fetcher.ly == fetcher.wy)
        fetcher.wy_cond = true;
    eval_lyc_intr();
}

void PPU::eval_lyc_intr() {
    if (!(fetcher.stat & 0x40))
        return;
    auto line = fetcher.ly;
    if (line == 153 && fetcher.lyc != 153)
        line = 0;
    if (line == fetcher.lyc)
        req_interrupt(LCD);
}

void PPU::update_stat() {
    u8 data = ((fetcher.ly == fetcher.lyc) << 2) | static_cast<u8>(mode);
    update_read_only(fetcher.stat, data, 0xF8);
}

void PPU::disable_lcd() {
    update_read_only(fetcher.stat, 0, 0xFC);
    std::fill(display.begin(), display.end(), (u8)0);
    new_frame();
    mode = PPU_State::OAM_SCAN;
    disabled = true;
}

void PPU::tick() { // tick for 4 t-cycles
    if (!(fetcher.lcdc & 0x80)) {
        if (disabled)
            return;
        return disable_lcd();
    }
    disabled = false;
    switch (mode) {
    case PPU_State::HBLANK:
        hblank();
        break;
    case PPU_State::VBLANK:
        vblank();
        break;
    case PPU_State::OAM_SCAN:
        oam_scan();
        oam_scan();
        break;
    case PPU_State::DRAWING:
        drawing();
        drawing();
        break;
    }
    update_stat();
}

void PPU::drawing() {
    dots += 2;
    if (!fetcher.bg_tick())
        return;

    if (push_to_display())
        return;
    if (push_to_display())
        return;
    if (fetcher.x_pos >= 160) {
        fetcher.bg_state = Fetcher_State::READ_TILE_ID;
        mode = PPU_State::HBLANK;
        if (fetcher.stat & 0x08)
            req_interrupt(LCD);
    }
}

void PPU::oam_scan() {
    add_sprite();
    dots += 2;
    if (dots < 80)
        return;

    mode = PPU_State::DRAWING;
}

void PPU::hblank() {
    dots += 4;
    if (dots < 456)
        return;

    dots = 0;
    increment_ly();
    if (fetcher.ly == 144) {
        mode = PPU_State::VBLANK;
        req_interrupt(VBLANK);
        if (fetcher.stat & 0x10)
            req_interrupt(LCD);
    } else {
        new_line();
        mode = PPU_State::OAM_SCAN;
        if (fetcher.stat & 0x20)
            req_interrupt(LCD);
        curr_sprite_location = OAM_S;
    }
}

void PPU::vblank() {
    dots += 4;
    if (dots < 456)
        return;

    dots = 0;
    increment_ly();
    if (fetcher.ly == 153) { // Next frame
        new_frame();
        mode = PPU_State::OAM_SCAN;
        if (fetcher.stat & 0x20)
            req_interrupt(LCD);
    }
}

bool PPU::push_to_display() {
    if (!fetcher.bg_count)
        return false;
    if (scx_discard) {
        fetcher.queue_bg <<= 2 * (fetcher.scx % 8);
        fetcher.bg_count -= (fetcher.scx % 8);
        scx_discard = false;
    }
    u16 bg_pixel = (fetcher.lcdc & 1) ? (fetcher.queue_bg >> 14)
                                      : 0; // pop 2 bits from the fifo
    fetcher.queue_bg <<= 2;
    --fetcher.bg_count;
    u16 select = bg_pixel,
        palette = fetcher.bgp; // select appropriate color from palette

    if (fetcher.sp_count) {
        u16 sp_pixel = (fetcher.queue_sp >> 28);
        fetcher.queue_sp <<= 4;
        --fetcher.sp_count;
        if ((fetcher.lcdc & 2) && (sp_pixel >> 2) &&
            !((sp_pixel & 1) && bg_pixel)) {
            select = sp_pixel >> 2;
            palette = (sp_pixel & 2) ? fetcher.obp1 : fetcher.obp0;
        }
    }

    u8 col = (palette >> (select * 2)) & 0x3;
    if (fetcher.x_pos >= 0 && fetcher.x_pos < SCREEN_WIDTH &&
        fetcher.ly < SCREEN_HEIGHT)
        display[pixel_pos(fetcher.ly, fetcher.x_pos)] = col;
    ++fetcher.x_pos;
    ++fetcher.tile_index;
    if (!fetcher.fetch_window)
        fetcher.check_window();
    return fetcher.check_sprite();
}

void PPU::add_sprite() {
    if (fetcher.sprite_buffer.size() >= 10)
        return;
    int ly_check = fetcher.ly + 16;
    int s_height = (fetcher.lcdc & 0x04) ? 16 : 8;
    int posy = fetcher.oam[curr_sprite_location + 0 - OAM_S];
    int posx = fetcher.oam[curr_sprite_location + 1 - OAM_S];

    // Add sprite if the below conditions apply
    if (posx > 0 && ly_check >= posy && ly_check < posy + s_height)
        fetcher.sprite_buffer.emplace_back(curr_sprite_location, fetcher.oam);
    curr_sprite_location += 4;
}

u8 PPU::read(u16 at) {
    switch (at) {
    case LCDC:
        return fetcher.lcdc;
    case STAT:
        return fetcher.stat;
    case SCY:
        return fetcher.scy;
    case SCX:
        return fetcher.scx;
    case LY:
        return fetcher.ly;
    case LYC:
        return fetcher.lyc;
    case DMA:
        return fetcher.dma;
    case BGP:
        return fetcher.bgp;
    case OBP0:
        return fetcher.obp0;
    case OBP1:
        return fetcher.obp1;
    case WY:
        return fetcher.wy;
    case WX:
        return fetcher.wx;
    default:
        return 0xFF;
    }
}

void PPU::write(u16 at, u8 data) {
    switch (at) {
    case LCDC:
        fetcher.lcdc = data;
        return;
    case STAT:
        update_read_only(fetcher.stat, data | 0x80, 0x07);
        return;
    case SCY:
        fetcher.scy = data;
        return;
    case SCX:
        fetcher.scx = data;
        return;
    case LY:
        return;
    case LYC:
        fetcher.lyc = data;
        return;
    case DMA:
        fetcher.dma = data;
        return;
    case BGP:
        fetcher.bgp = data;
        return;
    case OBP0:
        fetcher.obp0 = data;
        return;
    case OBP1:
        fetcher.obp1 = data;
        return;
    case WY:
        fetcher.wy = data;
        return;
    case WX:
        fetcher.wx = data;
        return;
    }
}