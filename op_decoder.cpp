#include "sm83.hpp"

void CPU::decode_opcode() {
    switch (op.byte) { // check for invalid ops or CB prefixed instructions
    case 0xDB:
    case 0xDD:
    case 0xE3:
    case 0xE4:
    case 0xEB:
    case 0xEC:
    case 0xED:
    case 0xF4:
    case 0xFC:
    case 0xFD: hard_lock = true; break;
    case 0xCB: blockCB(); break;
    }

    switch (op.bits(Masks::b76)) {
    case 0x0: block00(); break;
    case 0x1: block01(); break;
    case 0x2: block02(); break;
    case 0x3: block03(); break;
    }
}

void CPU::block00() {
    switch (op.byte) {
    case 0x00: nop(); break;
    case 0x10: stop(); break;
    case 0x07: rlca(); break;
    case 0x0F: rrca(); break;
    case 0x17: rla(); break;
    case 0x1F: rra(); break;
    case 0x27: daa(); break;
    case 0x2F: cpl(); break;
    case 0x37: scf(); break;
    case 0x3F: ccf(); break;
    case 0x18: jr_imm8(); break;
    }

    switch (op.bits(Masks::b3210)) {
    case 0x1: ld_r16_imm16(); break;
    case 0x2: ld_r16mem_a(); break;
    case 0xA: ld_a_r16mem(); break;
    case 0x8: ld_imm16_sp(); break;
    case 0x3: inc_r16(); break;
    case 0xB: dec_r16(); break;
    case 0x9: add_hl_r16(); break;
    }

    switch (op.bits(Masks::b210)) {
    case 0x4: inc_r8(); break;
    case 0x5: dec_r8(); break;
    case 0x6: ld_r8_imm8(); break;
    case 0x0: if (op.bits(Masks::b543) & 0b100) {  jr_cond_imm8(); }
    }
}

void CPU::block01() {
    switch (op.byte) {
    case 0x76: halt(); break;
    default: ld_r8_r8(); break;
    }
}

void CPU::block02() {
    switch (op.bits(Masks::b76543)) {
    case 0x10: add_a_r8(); break;
    case 0x11: adc_a_r8(); break;
    case 0x12: sub_a_r8(); break;
    case 0x13: sbc_a_r8(); break;
    case 0x14: and_a_r8(); break;
    case 0x15: xor_a_r8(); break;
    case 0x16: or_a_r8(); break;
    case 0x17: cp_a_r8(); break;
    }
}

void CPU::block03() {
}

void CPU::blockCB() {
}