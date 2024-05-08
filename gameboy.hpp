#pragma once

#include "memory.hpp"
#include "sm83.hpp"
#include "timer.hpp"
#include "debug.hpp"

struct GameBoy {
    Memory memory{};
    Timer timer{ &memory };
    Debugger debugger{};
    CPU sm83{ &memory, &timer, &debugger };

    void run_instruction() {
        sm83.fetch_opcode();
        sm83.decode_opcode();
        sm83.handle_interrupts();
    }
};