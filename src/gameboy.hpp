#pragma once

#include <SDL.h>
#include <chrono>

#include "ppu.hpp"
#include "sm83.hpp"
#include "debug.hpp"
#include "timer.hpp"
#include "memory.hpp"
#include "window_handler.hpp"

#define LOG

enum Button {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    A,
    B,
    SELECT,
    START,
};

struct GameBoy {
    bool enabled{};

    Window_Handler handler{
        "StarGB",
        SCREEN_HEIGHT, SCREEN_WIDTH, 3,
        0xA1EF8C, 0x3FAC95,
        0x446176, 0x2C2137
    };
    
    Memory   memory{};
    PPU      ppu{ &memory, &handler, handler_wrapper };
    Timer    timer{ &memory };
    CPU      sm83{ &memory, &ppu, &timer };

    Debugger debugger{ &memory, &sm83 };

    void start();
    void run_instruction();

    void no_boot_rom();
    void load_game(const string& path);
    void load_boot(const string& path);

    void handle_events();
    void set_button_on(const SDL_Scancode& sym);
    void set_button_off(const SDL_Scancode& sym);
};

// delta function to measure elapsed time
template <
    class result_t = std::chrono::milliseconds,
    class clock_t = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start) {
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}