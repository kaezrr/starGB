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

    u8 ly() { return io_reg[LY - IO_S]; }
    u8 wy() { return io_reg[WY - IO_S]; }
    u8 wx() { return io_reg[WX - IO_S]; }
    u8 scy() { return io_reg[SCY - IO_S]; }
    u8 scx() { return io_reg[SCX - IO_S]; }
    u8 lyc() { return io_reg[LYC - IO_S]; }
    u8 bgp() { return io_reg[BGP - IO_S]; }
    u8 dma() { return io_reg[DMA - IO_S]; }
    u8 obp0() { return io_reg[OBP0 - IO_S]; }
    u8 obp1() { return io_reg[OBP1 - IO_S]; }
    u8 lcdc() { return io_reg[LCDC - IO_S]; }
    u8 stat() { return io_reg[STAT - IO_S]; }

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
};