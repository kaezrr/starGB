#pragma once

#include <SDL.h>
#include "memory.hpp"
#include "constants.hpp"
#include "fetcher.hpp"

enum class PPU_State {
    HBLANK,
    VBLANK,
    OAM_SCAN,
    DRAWING,
};

struct PPU {
    Memory* memory{};
    Fetcher fetcher{ nullptr };

    bool scx_discard{};
    u16 curr_sprite_location{ OAM_S }, dots{ 0 };

    PPU_State mode{ PPU_State::OAM_SCAN };
    array<u32, SCREEN_HEIGHT * SCREEN_WIDTH> display{};
    array<u32, 4> colors{ GB_PALETTE_0, GB_PALETTE_1, GB_PALETTE_2, GB_PALETTE_3 };

	SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    PPU(Memory* mem_ptr, SDL_Renderer* rend, SDL_Texture* text);

    void reset_ly() { memory->io_reg[LY - IO_S] = 0; }
    u8 ly() { return memory->io_reg[LY - IO_S]; }
    u8 wy() { return memory->io_reg[WY - IO_S]; }
    u8 wx() { return memory->io_reg[WX - IO_S]; }
    u8 scy() { return memory->io_reg[SCY - IO_S]; }
    u8 scx() { return memory->io_reg[SCX - IO_S]; }
    u8 lyc() { return memory->io_reg[LYC - IO_S]; }
    u8 bgp() { return memory->io_reg[BGP - IO_S]; }
    u8 dma() { return memory->io_reg[DMA - IO_S]; }
    u8 obp0() { return memory->io_reg[OBP0 - IO_S]; }
    u8 obp1() { return memory->io_reg[OBP1 - IO_S]; }
    u8 lcdc() { return memory->io_reg[LCDC - IO_S]; }
    u8 stat() { return memory->io_reg[STAT - IO_S]; }
    u8 oam(u16 addr) { return memory->oam[addr - OAM_S]; }

    void req_interrupt(u8 intr) { memory->io_reg[IF - IO_S] |= intr; }
    size_t pixel_pos(int y, int x) { return (y * SCREEN_WIDTH) + x; }
    
    void tick();
    void add_sprite();
    void update_stat();
    void increment_ly();

    void oam_scan();
    void drawing();
    void hblank();
    void vblank();

    void load_texture();
    void push_to_display();
};