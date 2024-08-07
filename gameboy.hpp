#pragma once

#include <SDL.h>
#include <chrono>
#include <fstream>
#include <iostream>

#include "ppu.hpp"
#include "sm83.hpp"
#include "debug.hpp"
#include "timer.hpp"
#include "memory.hpp"

//#define LOG

struct GameBoy {
    PPU      ppu;
    Memory   memory{};
    Timer    timer{ &memory };
    CPU      sm83{ &memory, &timer, &ppu };
    bool enabled{};
#ifdef LOG
    Debugger debugger{};
#endif // DEBUG
    GameBoy(SDL_Renderer* renderer, SDL_Texture* texture);

    void run_instruction();
    void start();
    void no_boot_rom();
    void load_game(const string& path);
    void load_boot(const string& path);
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