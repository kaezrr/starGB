#include "constants.hpp"
#include "memory.hpp"


struct PPU {
    Memory* memory{};
    u32 sprite_buffer{};
    u16 bg_buffer{};

    PPU(Memory* mem_ptr);
};