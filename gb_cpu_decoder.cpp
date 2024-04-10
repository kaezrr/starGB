#include "gb_cpu.hpp"


u8 CPU::decode_opcode() {
    switch (opcode.byte) { // check for invalid opcodes
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
    }

    switch (opcode.get_family()) {
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

    case 0x07: // RLCA
    case 0x0F: // RRCA
    case 0x17: // RLA
    case 0x1F: // RCA

    case 0x27: // DAA
    case 0x2F: // CPL
    case 0x37: // SCF
    case 0x3F: // CCF   

    case 0x18: // JR imm8
    }

    switch (opcode.get_bit3210()) {
    case 0x1: // LD r16, imm16
        write_r16(opcode.get_bit54(), get_nextu16(), false);
        return 12;

    case 0x2: // LD [r16mem], A
        write_r16mem(opcode.get_bit54(), AF.hi);
        return 8;

    case 0xA: // LD A, [r16mem]
        AF.hi = read_r16mem(opcode.get_bit54());
        return 8;

    case 0x8: { // LD [imm16], SP
        u16 addr = get_nextu16();
        memory.write(addr, SP.full & 0xFF);
        memory.write(addr + 1, SP.full >> 8);
        return 20;
    }

    case 0x3: // INC r16
        write_r16(opcode.get_bit54(), read_r16(opcode.get_bit54(), false) + 1, false);
        return 8;

    case 0xB: // DEC r16
        write_r16(opcode.get_bit54(), read_r16(opcode.get_bit54(), false) - 1, false);
        return 8;

    case 0x9: // ADD HL, r16
    }

    switch (opcode.get_bit210()) {
    case 0x4: { // INC r8
        u8 reg = read_r8(opcode.get_bit543()) + 1;
        set_flag(Flags::Z, !reg);
        set_flag(Flags::N, false);
        set_flag(Flags::H, ((reg & 0xF) == 0x0));
        write_r8(opcode.get_bit543(), reg);
        return 4;
    }

    case 0x5: { // DEC r8
        u8 reg = read_r8(opcode.get_bit543()) - 1;
        set_flag(Flags::Z, !reg);
        set_flag(Flags::N, true);
        set_flag(Flags::H, ((reg & 0xF) == 0xF));
        write_r8(opcode.get_bit543(), reg);
        return 4;
    }

    case 0x6: // LD r8, imm8
        write_r8(opcode.get_bit543(), get_nextu8());
        return 8;

    case 0x0: 
        if (opcode.get_bit54() & 0b10) { // JR cond, imm8
        }
    }

    return 0;
}

u8 CPU::block1() {
    return 0;
}

u8 CPU::block2() {
    return 0;
}

u8 CPU::block3() {
    return 0;
}
