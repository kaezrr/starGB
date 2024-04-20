#pragma once

#include "memory.hpp"
#include "sm83.hpp"
#include "debug.hpp"

struct GameBoy {
    Memory memory{};
    Debugger debugger{};
    CPU sm83{ memory, debugger };

    void run_instruction() {
        sm83.fetch_opcode();
        sm83.decode_opcode();
    }
};