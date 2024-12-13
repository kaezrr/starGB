#pragma once

#include "constants.hpp"
#include <vector>

using std::vector;

enum class Fetcher_State {
    READ_TILE_ID,
    READ_TILE_DATA0,
    READ_TILE_DATA1,
    PUSH_TO_FIFO,
    PAUSED,
    SP_READ_TILE_ID,
    SP_READ_TILE_DATA0,
    SP_READ_TILE_DATA1,
    SP_PUSH_TO_FIFO,
};

struct Sprite {
    u8 posY{}, posX{}, tile_id{};

    u8 obj_priority{};
    u8 flipY{}, flipX{};
    u8 palette{}, used{};

    Sprite() = default;
    Sprite(u16 at, vector<u8>& oam);
};

struct Fetcher {
    Fetcher_State bg_state{Fetcher_State::READ_TILE_ID};
    Fetcher_State sp_state{Fetcher_State::SP_READ_TILE_ID};

    int x_pos{}, tile_index{};
    u32 queue_sp{}, sp_data{};
    u16 queue_bg{}, bg_data{};
    u16 window_line_counter{};
    u16 bg_tile_no{}, bg_count{}, sp_count{};

    Sprite curr_sp{};
    vector<Sprite> sprite_buffer;
    bool wy_cond{}, delay{}, sp_fetch{};
    bool fetch_window{}, increment_window{};

    u8 lcdc{}, stat{}, scy{}, scx{}, ly{}, lyc{}, dma{}, bgp{}, obp1{}, obp0{},
        wy{}, wx{};

    vector<u8> vram = vector<u8>(0x2000);
    vector<u8> oam = vector<u8>(0x00A0);

    Fetcher() {
        sprite_buffer.reserve(10);
        new_frame();
    }

    void bg_fetch_tile_no();
    void bg_fetch_tile_data(bool state);
    bool bg_push_to_fifo();
    void sp_fetch_tile_no();
    void sp_fetch_tile_data(bool state);
    void sp_push_to_fifo();

    bool bg_tick();
    void sp_tick();
    void new_line();
    void new_frame();
    void inc_windowline();

    void check_window();
    bool check_sprite();
};