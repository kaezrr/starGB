#include "gb_cpu_decoder.hpp"


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
        bool subtract = get_flag(Flags::N);
        if (get_flag(Flags::H) || (!subtract && (AF.hi & 0xF) > 0x9)) {
            correction |= 0x06;
        }
        if (get_flag(Flags::C) || (!subtract && AF.hi > 0x99)) {
            correction |= 0x60;
            set_flag(Flags::C, true);
        }

        AF.hi += subtract ? -correction : correction;
        set_flag(Flags::Z, AF.hi == 0);
        set_flag(Flags::H, false);
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

    case 0x18: {// JR imm8
        auto imm8 = get_nextu8();
        PC.full += static_cast<s8>(imm8);
        return 12;
    }
    }

    auto r = opcode.bits(Masks::b54);
    switch (opcode.bits(Masks::b3210)) {
    case 0x1: // LD r16, imm16
        write_r16(r, get_nextu16(), false);
        return 12;

    case 0x2: // LD [r16mem], A
        write_r16mem(r, AF.hi);
        return 8;

    case 0xA: // LD A, [r16mem]
        AF.hi = read_r16mem(r);
        return 8;

    case 0x8: { // LD [imm16], SP
        u16 addr = get_nextu16();
        memory.write(addr, SP.full & 0xFF);
        memory.write(addr + 1, SP.full >> 8);
        return 20;
    }

    case 0x3: // INC r16
        write_r16(r, read_r16(r, false) + 1, false);
        return 8;

    case 0xB: // DEC r16
        write_r16(r, read_r16(r, false) - 1, false);
        return 8;

    case 0x9: {// ADD HL, r16
        u16 reg = read_r16(r, false);
        u16 res = HL.full + reg;
        set_flag(Flags::N, false);
        set_flag(Flags::H, CFLAG_ADD(HL.full, reg, res, Masks::hf16));
        set_flag(Flags::C, CFLAG_ADD(HL.full, reg, res, Masks::cf16));
        HL.full = res;
        return 8;
    }
    }

    r = opcode.bits(Masks::b543);
    switch (opcode.bits(Masks::b210)) {
    case 0x4: { // INC r8
        u8 reg = read_r8(r);
        u8 res = reg + 1;
        set_flag(Flags::Z, !reg);
        set_flag(Flags::N, CFLAG_ADD(reg, 1, res, Masks::hf8));
        set_flag(Flags::H, CFLAG_ADD(reg, 1, res, Masks::cf8));
        write_r8(r, res);
        return 4;
    }

    case 0x5: { // DEC r8
        u8 reg = read_r8(r);
        u8 res = reg - 1;
        set_flag(Flags::Z, !reg);
        set_flag(Flags::N, CFLAG_SUB(reg, 1, res, Masks::hf8));
        set_flag(Flags::H, CFLAG_SUB(reg, 1, res, Masks::cf8));
        write_r8(r, res);
        return 4;
    }

    case 0x6: // LD r8, imm8
        write_r8(r, get_nextu8());
        return 8;

    case 0x0: // JR cond, imm8
        if (r & 0b100) {
            auto imm8 = static_cast<s8>(get_nextu8());
            if (!check_cond(opcode.bits(Masks::b43))) return 8;
            PC.full += imm8;
            return 12;
        }
    }

    return 4;
}

u8 CPU::block1() {
    switch (opcode.byte) {
    case 0x76:  // HALT
        return 4;
    default:    // LD r8, r8
        write_r8(opcode.bits(Masks::b543), read_r8(opcode.bits(Masks::b210)));
        if (opcode.bits(Masks::b543) == 6 || opcode.bits(Masks::b210) == 6) return 8; // check if r8 is [HL]
    }
    return 4;
}

u8 CPU::block2() {
    auto r8 = read_r8(opcode.bits(Masks::b210));
    u8 return_cycles = (opcode.bits(Masks::b210) == 6) ? 8 : 4; // check if r8 is [HL]

    switch (opcode.bits(Masks::b76543)) {
    case 0x10: { // ADD A, r8
        u32 res = AF.hi + r8;
        set_flag(Flags::Z, !res);
        set_flag(Flags::N, false);
        set_flag(Flags::H, CFLAG_ADD(AF.hi, r8, res, Masks::hf8));
        set_flag(Flags::C, CFLAG_ADD(AF.hi, r8, res, Masks::cf8));
        AF.hi = static_cast<u8>(res);
        break;
    }

    case 0x11: { // ADC A, r8
        u32 res = AF.hi + r8 + get_flag(Flags::C);
        set_flag(Flags::Z, !res);
        set_flag(Flags::N, false);
        set_flag(Flags::H, CFLAG_ADD(AF.hi, r8, res, Masks::hf8));
        set_flag(Flags::C, CFLAG_ADD(AF.hi, r8, res, Masks::cf8));
        AF.hi = static_cast<u8>(res);
        break;
    }

    case 0x12: { // SUB A, r8
        u32 res = AF.hi - r8;
        set_flag(Flags::Z, !res);
        set_flag(Flags::N, true);
        set_flag(Flags::H, CFLAG_SUB(AF.hi, r8, res, Masks::hf8));
        set_flag(Flags::C, CFLAG_SUB(AF.hi, r8, res, Masks::cf8));
        AF.hi = static_cast<u8>(res);
        break;
    }

    case 0x13: { // SBC A, r8
        u32 res = AF.hi - r8 - get_flag(Flags::C);
        set_flag(Flags::Z, !res);
        set_flag(Flags::N, true);
        set_flag(Flags::H, CFLAG_SUB(AF.hi, r8, res, Masks::hf8));
        set_flag(Flags::C, CFLAG_SUB(AF.hi, r8, res, Masks::cf8));
        AF.hi = static_cast<u8>(res);
        break;
    }

    case 0x14: { // AND A, r8
    }

    case 0x15: { // XOR A, r8
    }

    case 0x16: { //  OR A, r8
    }

    case 0x17: { //  CP A, r8
    }
    }
    return return_cycles;
}

u8 CPU::block3() {
    return 4;
}

u8 CPU::blockCB() {
    return 4;
}