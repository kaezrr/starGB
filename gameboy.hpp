#pragma once

#include "memory.hpp"
#include "sm83.hpp"
#include "timer.hpp"
#include "debug.hpp"
#include "ppu.hpp"

struct GameBoy {
    Memory   memory{};
    Debugger debugger{};
    Timer    timer{ &memory };
    PPU      ppu{ &memory };
    CPU      sm83{ &memory, &timer, &debugger };

    void run_instruction() {
        sm83.fetch_opcode();
        sm83.decode_opcode();
        sm83.handle_interrupts();
    }
};