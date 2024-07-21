#pragma once

#include "constants.hpp"
#include "memory.hpp"
#include <SDL.h>

struct Sprite {
    u8 posY{};
    u8 posX{};
    u8 tile_id{};

    bool obj_priority{};
    bool flipY{};
    bool flipX{};
    bool palette{};

    Sprite(u16 at, Memory& mem);
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
private:
    Memory* memory{};
    u32 queue_sp{};
    u16 queue_bg{};

    u16 bg_count{ 0 }, dots{ 0 }, x_pos{ 0 };
    u16 curr_sprite_location{ OAM_S };
    u16 window_line_counter{};

    u16 bg_data{};
    u32 sp_data{};
    u16 bg_tile_no{};
    u16 sp_tile_no{};

    PPU_State mode{ PPU_State::OAM_SCAN };
    Fetcher_State fstate{ Fetcher_State::READ_TILE_ID };

    vector<Sprite> sprite_buffer;
    vector<u32> colors{ GB_PALETTE_0, GB_PALETTE_1, GB_PALETTE_2, GB_PALETTE_3 };
    array<u32, SCREEN_HEIGHT* SCREEN_WIDTH> display{};

    bool wy_cond{}, wx_cond{};
	SDL_Renderer* renderer{};
    SDL_Texture* texture{};

public:
    PPU(Memory* mem_ptr, SDL_Renderer* rend, SDL_Texture* text);
    
    void tick();
    void add_sprite(u16 at);
    bool display_window();
    void update_lyc();
    void update_mode();
    void increment_ly();

    void oam_scan();
    void vblank();
    void hblank();
    void drawing();

    void bg_fetch_tile_no();
    void bg_fetch_tile_data(bool state);
    void bg_push_to_fifo();
    bool sp_fetch_tile_no();
    void sp_fetch_tile_data(bool state);
    void sp_push_to_fifo();
    void push_to_display();

    void load_texture();
};