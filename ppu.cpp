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


PPU::PPU(Memory* mem_ptr, SDL_Renderer* rend, SDL_Texture* text) 
    : memory{ mem_ptr }, renderer{ rend }, texture{ text } {
    sprite_buffer.reserve(10);
}

bool PPU::display_window() {
    return wx_cond && wy_cond && (memory->read(LCDC) & 0x20);
}

void PPU::update_mode() {
    u8 stat = memory->read(STAT);
    u8 mask = 0xFD & static_cast<u8>(mode);
    memory->write(STAT, stat & mask);
}

void PPU::update_lyc() {
    u8 stat = memory->read(STAT);
    u8 mask = 0xFC & ((memory->read(LY) == memory->read(LYC)) << 1);
    memory->write(STAT, stat & mask);
}

void PPU::increment_ly() {
    memory->write(LY, memory->read(LY) + 1);
}

void PPU::tick() {
    switch (mode) {
    case PPU_State::HBLANK: 
        hblank();
        break;
    case PPU_State::VBLANK: 
        vblank();
        break;
    case PPU_State::OAM_SCAN: // 2 oam_scans take 4 t-cycles
        oam_scan(); oam_scan();
        break;
    case PPU_State::DRAWING: 
        drawing();
        break;
    }
    update_mode();
}

void PPU::drawing() {
    switch (fstate) {
    case Fetcher_State::READ_TILE_ID: 
        bg_fetch_tile_no();
        sp_fetch_tile_no();
        fstate = Fetcher_State::READ_TILE_DATA0;
        break;
    case Fetcher_State::READ_TILE_DATA0: 
        bg_fetch_tile_data(false);
        sp_fetch_tile_data(false);
        fstate = Fetcher_State::READ_TILE_DATA1;
        break;
    case Fetcher_State::READ_TILE_DATA1:
        bg_fetch_tile_data(true);
        sp_fetch_tile_data(true);
        fstate = Fetcher_State::PUSH_TO_FIFO;
        break;
    case Fetcher_State::PUSH_TO_FIFO:
        bg_push_to_fifo();
        sp_push_to_fifo();
        fstate = Fetcher_State::READ_TILE_ID;
        break;
    }
    push_to_display();
    mode = (x_pos < 160) ? PPU_State::DRAWING : PPU_State::HBLANK;
}

void PPU::oam_scan() {
    add_sprite(curr_sprite_location);
    curr_sprite_location += 4;
    if (dots < 80) return;
    // Update Mode
    curr_sprite_location = OAM_S;
    x_pos = 0;
    mode = PPU_State::DRAWING;
    wy_cond = (memory->read(LY) == memory->read(WY));
}

void PPU::hblank() {
    dots += 4;

    if (dots < 456) return;
    dots = 0;
    increment_ly();
    // Update Mode
    mode = (memory->read(LY) < 144) ? PPU_State::OAM_SCAN : PPU_State::VBLANK;
    if (mode == PPU_State::VBLANK) {
        window_line_counter = 0;
        load_texture();
    }
}

void PPU::vblank() {
    dots += 4;

    if (dots < 456) return;
    dots = 0;
    increment_ly();
    // Update Mode
    mode = (memory->read(LY) < 153) ? PPU_State::VBLANK : PPU_State::OAM_SCAN;
    if (mode == PPU_State::VBLANK) window_line_counter = 0;
    else memory->write(LY, 0);
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

void PPU::bg_fetch_tile_no() {
    dots += 2;

    u8 lcdc   = memory->read(LCDC);
    u8 scy    = memory->read(SCY);
    u8 scx    = memory->read(SCX);
    u8 ly     = memory->read(LY);
    u16 tile_map = TILE_MAP1;
    if ((lcdc & 0x08) && !display_window() ||
        (lcdc & 0x40) && display_window())
        tile_map = TILE_MAP2;
    u16 addr = display_window() ? x_pos : (x_pos + (scx + 8)) & 0x1F;
    u16 offs = display_window() ? 32 * (window_line_counter / 8) : 32 * (((ly + scy) & 0xFF) / 8);
    addr &= 0x3FF;
    offs &= 0x3FF;
    bg_tile_no = tile_map + addr + offs;
}

bool PPU::sp_fetch_tile_no() {
    dots += 2;

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

void PPU::bg_fetch_tile_data(bool state) {
    dots += 2;

    u8  lcdc = memory->read(LCDC);
    u8  scy  = memory->read(SCY);
    u8  ly   = memory->read(LY);
    u8  addr = (u8)((lcdc & 0x10) ? 0x8000 + bg_tile_no * 16 : 0x8800 + static_cast<s8>(bg_tile_no) * 16);
    u16 offs = (u8)(display_window() ? 2 * (window_line_counter % 8) : 2 * ((ly + scy) % 8));
    if (!state) {
        u8 lsb = memory->read(addr + offs);
        for (u8 i = 0; i < 8; ++i)
            bg_data |= (lsb & (1 << i)) << i;
    } else {
        u8 msb = memory->read(addr + offs + 1);
        for (u8 i = 1; i < 8; ++i)
            bg_data |= (msb & (1 << i)) << (i + 1);
    }
}

void PPU::sp_fetch_tile_data(bool state) {
    dots += 2;

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

void PPU::bg_push_to_fifo() {
    dots += 2;

    if (bg_count) return;
    queue_bg = bg_data;
}

void PPU::sp_push_to_fifo() {
    dots += 2;

    u32 mask = ~queue_sp & (~queue_sp << 1) & 0x88888888;
    mask = (mask << 1) - (mask >> 3);
    queue_sp |= sp_data & mask;
}

void PPU::push_to_display() {
    if (!bg_count) return;
    u8 bg_pixel = (u8)(queue_bg >> 14);
    queue_bg <<= 2; --bg_count;
    u8 sp_pixel = (u8)(queue_sp >> 28);
    queue_sp <<= 4;

    u16 select, palette;
    if (!(sp_pixel & 0xC) || (sp_pixel & 0x1)) {
        select = bg_pixel;
        palette = BGP;
    } else {
        select = (sp_pixel >> 2) & 0x3;
        palette = (sp_pixel & 0x2) ? OBP1 : OBP0;
    }
    display[memory->read(LY) * SCREEN_HEIGHT + x_pos++] = colors[(memory->read(palette) >> (select * 2)) & 0x3];
}


void PPU::load_texture() {
    int pitch = SCREEN_WIDTH * SCREEN_HEIGHT;
    u32* pixels{};
	SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(pixels), &pitch);
    pixels = display.data();
    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
