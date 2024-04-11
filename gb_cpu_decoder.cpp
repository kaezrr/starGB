#include "gb_cpu.hpp"

namespace Masks {
    enum Mask {
        // Bit masks for opcode
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

u8 CPU::block0() {
    switch (opcode.byte) {
    case 0x00: // NOP
    case 0x10: // STOP
        return 4;

    case 0x07: // RLCA
        set_flag(Flags::Z, false);
        set_flag(Flags::N, false);
        set_flag(Flags::H, false);
        set_flag(Flags::C, AF.hi & 0x80);
        AF.hi = (AF.hi << 1) + AF.hi & 0x80;
        return 4;

    case 0x0F: // RRCA
        set_flag(Flags::Z, false);
        set_flag(Flags::N, false);
        set_flag(Flags::H, false);
        set_flag(Flags::C, AF.hi & 0x01);
        AF.hi = (AF.hi >> 1) + ((AF.hi & 0x01) << 7);
        return 4;

    case 0x17: {// RLA
        u8 prev_carry = get_flag(Flags::C);
        set_flag(Flags::Z, false);
        set_flag(Flags::N, false);
        set_flag(Flags::H, false);
        set_flag(Flags::C, AF.hi & 0x80);
        AF.hi = (AF.hi << 1) + prev_carry;
        return 4;
    }

    case 0x1F: { // RRA
        u8 prev_carry = get_flag(Flags::C);
        set_flag(Flags::Z, false);
        set_flag(Flags::N, false);
        set_flag(Flags::H, false);
        set_flag(Flags::C, AF.hi & 0x01);
        AF.hi = (AF.hi >> 1) + (prev_carry << 7);
        return 4;
    }

    case 0x27: { // DAA
        u8 correction = 0;
        bool carry = false, subtract = get_flag(Flags::N);
        if (get_flag(Flags::H) || (!subtract && (AF.hi & 0xF) > 0x9)) {
            correction |= 0x06;
        }
        if (get_flag(Flags::C) || (!subtract && AF.hi > 0x99)) {
            correction |= 0x60;
            carry = true;
        }

        AF.hi += subtract ? -correction : correction;
        set_flag(Flags::Z, AF.hi == 0);
        set_flag(Flags::H, false);
        set_flag(Flags::C, carry);
        return 4;
    }

    case 0x2F: // CPL
        AF.hi = ~AF.hi;
        set_flag(Flags::N, true);
        set_flag(Flags::H, true);
        return 4;

    case 0x37: // SCF
        set_flag(Flags::C, true);
        set_flag(Flags::N, false);
        set_flag(Flags::H, false);
        return 4;

    case 0x3F: // CCF   
        set_flag(Flags::C, !get_flag(Flags::C));
        set_flag(Flags::N, false);
        set_flag(Flags::H, false);
        return 4;

    case 0x18: // JR imm8
        PC.full += static_cast<s8>(get_nextu8());
        return 12;
    }

    switch (opcode.bits(Masks::b3210)) {
    case 0x1: // LD r16, imm16
        write_r16(opcode.bits(Masks::b54), get_nextu16(), false);
        return 12;

    case 0x2: // LD [r16mem], A
        write_r16mem(opcode.bits(Masks::b54), AF.hi);
        return 8;

    case 0xA: // LD A, [r16mem]
        AF.hi = read_r16mem(opcode.bits(Masks::b54));
        return 8;

    case 0x8: { // LD [imm16], SP
        u16 addr = get_nextu16();
        memory.write(addr, SP.full & 0xFF);
        memory.write(addr + 1, SP.full >> 8);
        return 20;
    }

    case 0x3: // INC r16
        write_r16(opcode.bits(Masks::b54), read_r16(opcode.bits(Masks::b54), false) + 1, false);
        return 8;

    case 0xB: // DEC r16
        write_r16(opcode.bits(Masks::b54), read_r16(opcode.bits(Masks::b54), false) - 1, false);
        return 8;

    case 0x9: {// ADD HL, r16
        u16 reg = read_r16(opcode.bits(Masks::b54), false);
        u16 res = HL.full + reg;
        set_flag(Flags::N, false);
        set_flag(Flags::H, CFLAG_ADD(HL.full, reg, res, Masks::hf16));
        set_flag(Flags::C, CFLAG_ADD(HL.full, reg, res, Masks::cf16));
        HL.full = res;
        return 8;
    }
    }

    switch (opcode.bits(Masks::b210)) {
    case 0x4: { // INC r8
        u8 reg = read_r8(opcode.bits(Masks::b543));
        u8 res = reg + 1;
        set_flag(Flags::Z, !reg);
        set_flag(Flags::N, CFLAG_ADD(reg, 1, res, Masks::hf8));
        set_flag(Flags::H, CFLAG_ADD(reg, 1, res, Masks::cf8));
        write_r8(opcode.bits(Masks::b543), res);
        return 4;
    }

    case 0x5: { // DEC r8
        u8 reg = read_r8(opcode.bits(Masks::b543));
        u8 res = reg - 1;
        set_flag(Flags::Z, !reg);
        set_flag(Flags::N, CFLAG_SUB(reg, 1, res, Masks::hf8));
        set_flag(Flags::H, CFLAG_SUB(reg, 1, res, Masks::cf8));
        write_r8(opcode.bits(Masks::b543), res);
        return 4;
    }

    case 0x6: // LD r8, imm8
        write_r8(opcode.bits(Masks::b543), get_nextu8());
        return 8;

    case 0x0: 
        if (opcode.bits(Masks::b54) & 0b10) { // JR cond, imm8
            auto imm8 = static_cast<s8>(get_nextu8());
            if (!check_cond(opcode.bits(Masks::b43))) return 8;
            PC.full += imm8;
            return 12;
        }
    }

    return 4;
}

u8 CPU::block1() {
    return 4;
}

u8 CPU::block2() {
    return 4;
}

u8 CPU::block3() {
    return 4;
}

u8 CPU::blockCB() {
    return 4;
}
