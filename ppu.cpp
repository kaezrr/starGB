#include "ppu.hpp"

PPU::PPU(Memory* mem_ptr, SDL_Renderer* rend, SDL_Texture* text) 
    : memory{ mem_ptr }, renderer{ rend }, texture{ text } {
    sprite_buffer.reserve(10);
}

bool PPU::display_window() {
    return wx_cond && wy_cond && (memory->read(LCDC) & 0x20);
}

void PPU::update_stat() {
    u8 stat = memory->read(STAT);
    u8 mask1 = 0xFB & ((memory->read(LY) == memory->read(LYC)) << 1);
    u8 mask2 = 0xFC & static_cast<u8>(mode);
    memory->write(STAT, stat & mask1 & mask2);
}

void PPU::increment_ly() {
    memory->write(LY, memory->read(LY) + 1);
}

void PPU::tick() { // tick for 4 t-cycles
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
        if (bg_push_to_fifo()) 
            fstate = Fetcher_State::READ_TILE_ID;
        break;
    }
    push_to_display();
    push_to_display();
    if (x_pos == 160) {
        bg_count = 0;
        fstate = Fetcher_State::READ_TILE_ID;
        mode = PPU_State::HBLANK;
    }
}

void PPU::oam_scan() {
    dots += 4;
    if (dots < 80) return;
    x_pos = 0;
    mode = PPU_State::DRAWING;
}

void PPU::hblank() {
    dots += 4;
    if (dots < 456) return;
    dots = 0; 
    increment_ly();
    if (memory->read(LY) == 144) {
        load_texture();
        mode = PPU_State::VBLANK;
    } else {
        mode = PPU_State::OAM_SCAN;
    }
}

void PPU::vblank() {
    dots += 4;
    if (dots < 456) return;
    dots = 0; 
    increment_ly();
    if (memory->read(LY) == 153) {
        memory->write(LY, 0);
        mode = PPU_State::OAM_SCAN;
    }
}

void PPU::push_to_display() {
    if (!bg_count) return;
    u8 bg_pixel = (u8)(queue_bg >> 14); // pop 2 bits from the front
    queue_bg <<= 2; --bg_count;
    u16 select = bg_pixel, palette = BGP; // select appropriate color from palette
    display[memory->read(LY) * SCREEN_WIDTH + x_pos++] = colors[(memory->read(palette) >> (select * 2)) & 0x3];
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
