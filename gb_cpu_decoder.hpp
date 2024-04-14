#pragma once

#include "gb_cpu.hpp"

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

inline bool CFLAG_ADD(u32 a, u32 b, u32 res, u32 mask) {
    return ((a & b) | (a & ~res) | (b & ~res) | mask);
}

inline bool CFLAG_SUB(u32 a, u32 b, u32 res, u32 mask) {
    return CFLAG_ADD(res, b, a, mask);
}


u8 CPU::decode_opcode() {
    switch (opcode.byte) { // check for invalid opcodes or CB prefixed instructions
    case 0xDB:
    case 0xDD:
    case 0xE3:
    case 0xE4:
    case 0xEB:
    case 0xEC:
    case 0xED:
    case 0xF4:
    case 0xFC:
    case 0xFD:
        hard_lock = true; return 4;
    case 0xCB: 
        return blockCB();
    }

    switch (opcode.bits(Masks::b76)) {
    case 0x0:
        return block0();
    case 0x1:
        return block1();
    case 0x2:
        return block2();
    case 0x3:
        return block3();
    }
    return 4;
}
