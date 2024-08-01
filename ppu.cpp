#include "ppu.hpp"

PPU::PPU(Memory* mem_ptr, SDL_Renderer* rend, SDL_Texture* text) 
    : memory{ mem_ptr }, renderer{ rend }, texture{ text } {
    sprite_buffer.reserve(10);
}

inline size_t PPU::pixel_pos(int y, int x) {
    return (y * SCREEN_WIDTH) + x;
}

inline u8 PPU::ly() {
    return memory->read(LY);
}

inline u8 PPU::wy() {
    return memory->read(WY);
}

inline u8 PPU::wx() {
    return memory->read(WX);
}

inline u8 PPU::scy() {
    return memory->read(SCY);
}

inline u8 PPU::scx() {
    return memory->read(SCX);
}

bool PPU::display_window() {
    return wx_cond && wy_cond && (memory->read(LCDC) & 0x20);
}

void PPU::push_to_display() {
    if (!bg_count) return;
    u8 bg_pixel = (u8)(queue_bg >> 14); // pop 2 bits from the front
    queue_bg <<= 2; --bg_count;

    u8 ly = memory->read(LY);
    u16 select = bg_pixel, palette = BGP; // select appropriate color from palette
    u32 col = colors[(memory->read(palette) >> (select * 2)) & 0x3];
    display[pixel_pos(ly, x_pos++)] = col;
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

void PPU::update_stat() {
    u8 stat = memory->read(STAT);
    u8 mask1 = 0xFB | ((memory->read(LY) == memory->read(LYC)) << 2);
    u8 mask2 = 0xFC | static_cast<u8>(mode);
    memory->write(STAT, stat & mask1 & mask2);
}

void PPU::increment_ly() {
    memory->write(LY, ly() + 1);
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
        if (delay) {
            fstate = Fetcher_State::READ_TILE_ID;
            bg_data = 0; delay = false; 
        } else if (bg_push_to_fifo()) {
            fstate = Fetcher_State::READ_TILE_ID;
            tile_index += 8;
        }
        break;
    }
    push_to_display();
    push_to_display();
    if (x_pos == 160) {
        fstate = Fetcher_State::READ_TILE_ID;
        mode = PPU_State::HBLANK;
    }
}

void PPU::oam_scan() {
    dots += 4;
    if (dots < 80) return;

    // Reset all drawing stuff
    x_pos = 0; tile_index = 0;
    queue_bg = 0; queue_sp = 0;
    bg_data = 0; bg_count = 0;
    mode = PPU_State::DRAWING;
    delay = true;
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

void PPU::bg_fetch_tile_no() {
    u16 tile_x = ((scx() + tile_index) & 0xFF) / 8;
    u16 tile_y = ((scy() + ly()) & 0xFF) / 8;
    bg_tile_no = memory->read(TILE_MAP1 + (32 * tile_y) + tile_x);
}

void PPU::bg_fetch_tile_data(bool state) {
    u16 addr = 0x8000 + (bg_tile_no * 16);
    u16 offs = ((ly() + scy()) % 8) * 2;
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

