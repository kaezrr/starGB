#pragma once

#include <SDL.h>
#include "memory.hpp"
#include "constants.hpp"

struct Sprite {
    u8 posY{};
    u8 posX{};
    u8 tile_id{};

    bool obj_priority{};
    bool flipY{};
    bool flipX{};
    bool palette{};

    Sprite(u16 at, Memory* mem);
};

enum class PPU_State {
    HBLANK,
    VBLANK,
    OAM_SCAN,
    DRAWING,
};

enum class Fetcher_State {
    READ_TILE_ID,
    READ_TILE_DATA0,
    READ_TILE_DATA1,
    PUSH_TO_FIFO,
};


struct PPU {
    Memory* memory{};
    u32 queue_sp{};
    u16 queue_bg{};

    u16 curr_sprite_location{ OAM_S }, window_line_counter{ 0 };
    u16 bg_count{ 0 }, sp_count{ 0 }, dots{ 0 }, x_pos{ 0 }, tile_index{ 0 };

    u16 bg_data{};
    u32 sp_data{};
    u16 bg_tile_no{};
    u16 sp_tile_no{};
    bool wy_cond{}, delay{}, fetch_window{}, increment_window{}, scx_discard{};

    PPU_State mode{ PPU_State::OAM_SCAN };
    Fetcher_State fstate{ Fetcher_State::READ_TILE_ID };

    vector<Sprite> sprite_buffer;
    array<u32, 4> colors{ GB_PALETTE_0, GB_PALETTE_1, GB_PALETTE_2, GB_PALETTE_3 };
    array<u32, SCREEN_HEIGHT * SCREEN_WIDTH> display{};

	SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    PPU(Memory* mem_ptr, SDL_Renderer* rend, SDL_Texture* text);

    inline u8 ly() 		{ return memory->io_reg[LY - IO_S]; }
    inline u8 wy() 		{ return memory->io_reg[WY - IO_S]; }
    inline u8 wx() 		{ return memory->io_reg[WX - IO_S]; }
    inline u8 scy() 	{ return memory->io_reg[SCY - IO_S]; }
    inline u8 scx() 	{ return memory->io_reg[SCX - IO_S]; }
    inline u8 lyc() 	{ return memory->io_reg[LYC - IO_S]; }
    inline u8 bgp() 	{ return memory->io_reg[BGP - IO_S]; }
    inline u8 dma() 	{ return memory->io_reg[DMA - IO_S]; }
    inline u8 obp0() 	{ return memory->io_reg[OBP0 - IO_S]; }
    inline u8 obp1() 	{ return memory->io_reg[OBP1 - IO_S]; }
    inline u8 lcdc() 	{ return memory->io_reg[LCDC - IO_S]; }
    inline u8 stat() 	{ return memory->io_reg[STAT - IO_S]; }
    inline u8 oam(u16 addr) { return memory->oam[addr - OAM_S]; }
    inline u8 vram(u16 addr) { return memory->vram[addr - VRAM_S]; }

    inline size_t pixel_pos(int y, int x) { return (y * SCREEN_WIDTH) + x; }
    inline void req_interrupt(u8 intr) { memory->io_reg[IF - IO_S] |= intr; }
    
    void tick();
    void update_stat();
    void increment_ly();
    void load_texture();
    bool display_window();
    void add_sprite();

    void oam_scan();
    void drawing();
    void hblank();
    void vblank();

    void bg_fetch_tile_no();
    void bg_fetch_tile_data(bool state);
    bool bg_push_to_fifo();

    bool sp_fetch_tile_no();
    void sp_fetch_tile_data(bool state);
    void sp_push_to_fifo();

    void push_to_display();
};