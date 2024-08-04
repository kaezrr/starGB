#include "ppu.hpp"
#include <iostream>

PPU::PPU(Memory* mem_ptr, SDL_Renderer* rend, SDL_Texture* text) 
    : memory{ mem_ptr }, renderer{ rend }, texture{ text } {
    sprite_buffer.reserve(10);
}

void PPU::increment_ly() { 
    ++memory->io_reg[LY - IO_S]; 
    if (stat() & 0x40 && ly() == lyc()) req_interrupt(LCD);
    window_line_counter += increment_window; increment_window = false;
}

void PPU::update_stat() {
    u8 data = ((ly() == lyc()) << 2) | static_cast<u8>(mode);
    Memory::update_read_only(memory->io_reg[STAT - IO_S], data, 0xF8);
}

bool PPU::display_window() {
    return (x_pos >= wx() - 7) && wy_cond && (lcdc() & 0x20);
}

void PPU::load_texture() {
    int pitch = SCREEN_WIDTH * 4;
    u32* pixels{ nullptr };
    SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
    std::memcpy(pixels, display.data(), pitch * SCREEN_HEIGHT);
    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}


void PPU::tick() { // tick for 4 t-cycles
    if (!(lcdc() & 0x80)) return;
    switch (mode) {
    case PPU_State::HBLANK: hblank();
        break;
    case PPU_State::VBLANK: vblank();
        break;
    case PPU_State::OAM_SCAN: oam_scan();
        break;
    case PPU_State::DRAWING: drawing(); drawing();
        break;
    }
    if (ly() == wy()) wy_cond = true;
    update_stat();
}

void PPU::drawing() {
    dots += 2;
    switch (fstate) {
    case Fetcher_State::READ_TILE_ID: 
        bg_fetch_tile_no();
        fstate = Fetcher_State::READ_TILE_DATA0;
        break;

    case Fetcher_State::READ_TILE_DATA0: 
        bg_fetch_tile_data(false);
        fstate = Fetcher_State::READ_TILE_DATA1;
        break;

    case Fetcher_State::READ_TILE_DATA1:
        bg_fetch_tile_data(true);
        fstate = Fetcher_State::PUSH_TO_FIFO;
        break;

    case Fetcher_State::PUSH_TO_FIFO:
        if (delay) {
            fstate = Fetcher_State::READ_TILE_ID;
            delay = false; bg_data = 0;
        } else if (bg_push_to_fifo()) {
            fstate = Fetcher_State::READ_TILE_ID;
            tile_index += 8;
        }
        break;
    }

    push_to_display();
    push_to_display();
    if (x_pos >= 160) {
        fstate = Fetcher_State::READ_TILE_ID;
        mode = PPU_State::HBLANK;
        if (stat() & 0x08) req_interrupt(LCD);
    }
}

void PPU::oam_scan() {
    add_sprite();
    add_sprite();
    dots += 4;
    if (dots < 80) return;

    // Reset all fetcher properties
    mode = PPU_State::DRAWING;
    x_pos = 0; tile_index = 0;
    bg_data = 0; bg_count = 0;
    queue_bg = 0; queue_sp = 0;
    delay = true; scx_discard = true;
    fetch_window = false;
}

void PPU::hblank() {
    dots += 4;
    if (dots < 456) return;
    dots = 0; 
    increment_ly();
    if (ly() == 144) {
        load_texture();
        window_line_counter = 0;
        mode = PPU_State::VBLANK;
        req_interrupt(VBLANK);
        if (stat() & 0x10) req_interrupt(LCD);
    } else {
        mode = PPU_State::OAM_SCAN;
        if (stat() & 0x20) req_interrupt(LCD);
        curr_sprite_location = OAM_S;
        sprite_buffer.clear();
    }
}

void PPU::vblank() {
    dots += 4;
    if (dots < 456) return;
    dots = 0; 
    increment_ly();
    if (ly() == 153) { // Next frame
        memory->io_reg[LY - IO_S] = 0;
        wy_cond = false; mode = PPU_State::OAM_SCAN;
        if (stat() & 0x20) req_interrupt(LCD);
        curr_sprite_location = OAM_S;
        sprite_buffer.clear();
    }
}
