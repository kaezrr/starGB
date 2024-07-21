#pragma once

#include "memory.hpp"
#include "sm83.hpp"
#include "timer.hpp"
#include "debug.hpp"
#include "ppu.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <SDL.h>

struct GameBoy {
    PPU      ppu;
    Memory   memory{};
    Debugger debugger{};
    Timer    timer{ &memory };
    CPU      sm83{ &memory, &timer, &ppu };

    bool enabled{};

    GameBoy(SDL_Renderer* renderer, SDL_Texture* texture);

    void run_instruction();
    void start();
    void no_boot_rom();
    void load_game(const string& path);
};

// delta function to measure elapsed time
template <
    class result_t = std::chrono::milliseconds,
    class clock_t = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}