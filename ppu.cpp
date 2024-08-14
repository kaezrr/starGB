#include "ppu.hpp"

PPU::PPU(Memory* mem_ptr, void* instance, fn_type func) 
    : memory{ mem_ptr }, fetcher{ mem_ptr }, renderer{ instance, func } {}

void PPU::increment_ly() { 
    ++memory->io_reg[LY - IO_S]; 
    fetcher.inc_windowline();
    if (ly() == wy()) fetcher.wy_cond = true;
    if (stat() & 0x40 && ly() == lyc()) req_interrupt(LCD);
}

void PPU::update_stat() {
    u8 data = ((ly() == lyc()) << 2) | static_cast<u8>(mode);
    Memory::update_read_only(memory->io_reg[STAT - IO_S], data, 0xF8);
}

void PPU::disable_lcd() {
    memory->io_reg[LY - IO_S] = 0;
    memory->io_reg[STAT - IO_S] = 0;
    std::fill(display.begin(), display.end(), (u8)0);
    fetcher.new_frame(); renderer.call(display.data());
    dots = 0; mode = PPU_State::DRAWING;
    disabled = true;
}

void PPU::tick() { // tick for 4 t-cycles
    if (!(lcdc() & 0x80)) {
        if (disabled) return;
        return disable_lcd();
    } 

    disabled = false;

    switch (mode) {
    case PPU_State::HBLANK: hblank();
        break;
    case PPU_State::VBLANK: vblank();
        break;
    case PPU_State::OAM_SCAN: oam_scan(); oam_scan();
        break;
    case PPU_State::DRAWING: drawing(); drawing();
        break;
    }
    update_stat();
}

void PPU::drawing() {
    dots += 2;
    if(!fetcher.bg_tick()) return;

    if(push_to_display()) return;
    if(push_to_display()) return;
    if (fetcher.x_pos >= 160) {
        fetcher.bg_state = Fetcher_State::READ_TILE_ID;
        mode = PPU_State::HBLANK;
        if (stat() & 0x08) req_interrupt(LCD);
    }
}

void PPU::oam_scan() {
    add_sprite();
    dots += 2;
    if (dots < 80) return;

    scx_discard = true;
    mode = PPU_State::DRAWING;
}

void PPU::hblank() {
    dots += 4;
    if (dots < 456) return;

    dots = 0; 
    increment_ly();
    if (ly() == 144) {
        mode = PPU_State::VBLANK;
        req_interrupt(VBLANK);
        if (stat() & 0x10) req_interrupt(LCD);
    } else {
        fetcher.new_line();
        mode = PPU_State::OAM_SCAN;
        if (stat() & 0x20) req_interrupt(LCD);
        curr_sprite_location = OAM_S;
    }
}

void PPU::vblank() {
    dots += 4;
    if (dots < 456) return;

    dots = 0; 
    increment_ly();
    if (ly() == 153) { // Next frame
        reset_ly(); fetcher.new_frame();
        renderer.call(display.data());
        mode = PPU_State::OAM_SCAN;
        curr_sprite_location = OAM_S;
        if (stat() & 0x20) req_interrupt(LCD);
    }
}

bool PPU::push_to_display() {
    if (!fetcher.bg_count) return false;
    if (scx_discard) {
        fetcher.queue_bg <<= 2 * (scx() % 8);
        scx_discard = false; dots += scx() % 8;
    }
    u16 bg_pixel = (lcdc() & 1) ? (fetcher.queue_bg >> 14) : 0; // pop 2 bits from the fifo
    fetcher.queue_bg <<= 2; --fetcher.bg_count;
    u16 select = bg_pixel, palette = bgp(); // select appropriate color from palette

    if (fetcher.sp_count) {
        u16 sp_pixel = (fetcher.queue_sp >> 28);
        fetcher.queue_sp <<= 4; --fetcher.sp_count;
        if ((lcdc() & 2) && (sp_pixel >> 2) && !((sp_pixel & 1) && bg_pixel)) {
            select = sp_pixel >> 2;
            palette = (sp_pixel & 2) ? obp1() : obp0();
        }
    }

    u8 col = (palette >> (select * 2)) & 0x3;
    if (fetcher.x_pos >= 0 && fetcher.x_pos < 160) 
        display[pixel_pos(ly(), fetcher.x_pos)] = col;
    ++fetcher.x_pos; fetcher.check_window();
    return fetcher.check_sprite();
}

void PPU::add_sprite() {
    if (fetcher.sprite_buffer.size() >= 10) return;
    int ly_check = ly() + 16;
    int s_height = (lcdc() & 0x04) ? 16 : 8;
    int posy = oam(curr_sprite_location + 0);
    int posx = oam(curr_sprite_location + 1);

    // Add sprite if the below conditions apply
    if (posx > 0 && ly_check >= posy && ly_check < posy + s_height)
        fetcher.sprite_buffer.emplace_back(curr_sprite_location, memory);
    curr_sprite_location += 4;
}


