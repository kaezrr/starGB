#pragma once
#include "constants.hpp"
#include <bit>

namespace Masks {
    enum Mask {
        // Bit masks for opcode
        b76543  = 0xF8,
        b3210   = 0x0F,
        b210    = 0x07,
        b543    = 0x38,
        b54     = 0x30,
        b43     = 0x18,
        b76     = 0xC0,
        // Bit masks for flag
        hf8     = 0x08,     // half carry flag 8 bits
        cf8     = 0x80,     // full carry flag 8 bits
        hf16    = 0x0800,   // half carry flag 16 bits
        cf16    = 0x8000,   // full carry flag 16 bits
    };
}

enum Flag {
    Z = 0x80,
    N = 0x40,
    H = 0x20,
    C = 0x10,
};

union Registers {
    u16 full;
    struct { u8 lo, hi; };
};

struct Opcode {
    u8 byte{};
    inline u8 bits(u8 mask) const {
        return (byte & mask) >> std::countr_zero(mask);
    }
};

inline bool CFLAG_ADD(u32 a, u32 b, u32 res, u32 mask) {
    return (((a & b) | (a & ~res) | (b & ~res)) & mask);
}

inline bool CFLAG_SUB(u32 a, u32 b, u32 res, u32 mask) {
    return CFLAG_ADD(res, b, a, mask);
}

