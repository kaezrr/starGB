#pragma once

#include "memory.hpp"
#include "sm83.hpp"
#include "timer.hpp"

struct GameBoy {
    Memory memory{};
    Timer timer{ &memory };
    CPU sm83{ &memory, &timer };

    void run_instruction() {
        sm83.fetch_opcode();
        sm83.decode_opcode();
    }
};