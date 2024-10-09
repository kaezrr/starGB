#pragma once

#include <SDL.h>
#include "callback.hpp"
#include "constants.hpp"
#include "fetcher.hpp"
#include "window_handler.hpp"
#include <array>
#include <vector>

using std::array;
using std::vector;

enum class PPU_State {
    HBLANK,
    VBLANK,
    OAM_SCAN,
    DRAWING,
};

struct PPU {
    Fetcher fetcher;
    bool scx_discard{}, disabled{};
    u16 curr_sprite_location{ OAM_S }, dots{ 0 };

    PPU_State mode{ PPU_State::OAM_SCAN };
    array<u32, 4> colors{};
    vector<u8> display = vector<u8>(SCREEN_HEIGHT * SCREEN_WIDTH);

    Window_Handler screen{};
    CallBack renderer{&screen, handler_wrapper};

    vector<u8>& io_reg;
    PPU(vector<u8> &io_ptr)
        : io_reg{io_ptr}, fetcher{io_ptr}, renderer{&screen, handler_wrapper} {
        new_frame();
    }

    void req_interrupt(u8 intr) { io_reg[IF - IO_S] |= intr; }
    size_t pixel_pos(int y, int x) { return (y * SCREEN_WIDTH) + x; }
    
    void tick();
    void disable_lcd();
    void add_sprite();
    void update_stat();
    void increment_ly();
    void eval_lyc_intr();

    void oam_scan();
    void drawing();
    void hblank();
    void vblank();

    void new_frame();
    void new_line();

    bool push_to_display();

    u8 read(u16 at);
    void write(u16 at, u8 data);
};