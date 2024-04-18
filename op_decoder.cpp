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
    case 0xFD: hard_lock = true;
        break;
    case 0xCB: blockCB();
        break;
    }

    switch (op.bits(Masks::b76)) {
    case 0x00: block00();
        break;
    case 0x01: block01();
        break;
    case 0x02: block02();
        break;
    case 0x03: block03();
        break;
    }
}

void CPU::block00() {
    switch (op.byte) {
    case 0x00: nop();
        break;
    case 0x10: stop();
        break;
    case 0x07: rlca();
        break;
    case 0x0F: rrca();
        break;
    case 0x17: rla();
        break;
    case 0x1F: rra();
        break;
    case 0x27: daa();
        break;
    case 0x2F: cpl();
        break;
    case 0x37: scf();
        break;
    case 0x3F: ccf();
        break;
    case 0x18: jr_imm8();
        break;
    }

    switch (op.bits(Masks::b3210)) {
    case 0x01: ld_r16_imm16();
        break;
    case 0x02: ld_r16mem_a();
        break;
    case 0x0A: ld_a_r16mem();
        break;
    case 0x08: ld_imm16_sp();
        break;
    case 0x03: inc_r16();
        break;
    case 0x0B: dec_r16();
        break;
    case 0x09: add_hl_r16();
        break;
    }

    switch (op.bits(Masks::b210)) {
    case 0x04: inc_r8();
        break;
    case 0x05: dec_r8();
        break;
    case 0x06: ld_r8_imm8();
        break;
    case 0x00: jr_cond_imm8(); 
        break;
    }
}

void CPU::block01() {
    switch (op.byte) {
    case 0x76: halt();
        break;
    default: ld_r8_r8();
        break;
    }
}

void CPU::block02() {
    switch (op.bits(Masks::b76543)) {
    case 0x10: add_a_r8();
        break;
    case 0x11: adc_a_r8();
        break;
    case 0x12: sub_a_r8();
        break;
    case 0x13: sbc_a_r8();
        break;
    case 0x14: and_a_r8();
        break;
    case 0x15: xor_a_r8();
        break;
    case 0x16: or_a_r8();
        break;
    case 0x17: cp_a_r8();
        break;
    }
}

void CPU::block03() {
    switch (op.byte) {
    case 0xC6: add_a_imm8();
        break;
    case 0xCE: adc_a_imm8();
        break;
    case 0xD6: sub_a_imm8();
        break;
    case 0xDE: sbc_a_imm8();
        break;
    case 0xE6: and_a_imm8();
        break;
    case 0xEE: xor_a_imm8();
        break;
    case 0xF6: or_a_imm8();
        break;
    case 0xFE: cp_a_imm8();
        break;
    case 0xE2: ldh_c_a();
        break;
    case 0xE0: ldh_imm8_a();
        break;
    case 0xEA: ld_imm16_a();
        break;
    case 0xF2: ldh_a_c();
        break;
    case 0xF0: ldh_a_imm8();
        break;
    case 0xFA: ld_a_imm16();
        break;
    case 0xE8: add_sp_imm8();
        break;
    case 0xF8: ld_hl_sp_plus_imm8();
        break;
    case 0xCD: call_imm16();
        break;
    case 0xF9: ld_sp_hl();
        break;
    case 0xC3: jp_imm16();
        break;
    case 0xE9: jp_hl();
        break;
    case 0xC9: ret();
        break;
    case 0xD9: reti();
        break;
    case 0xF3: di();
        break;
    case 0xFB: ei();
        break;
    }
    if (!(op.byte & 0x20)) {
        switch(op.bits(Masks::b210)) {
        case 0x00: ret_cond();
            break;
        case 0x02: jp_cond_imm16();
            break;
        case 0x04: call_cond_imm16();
            break;
        }
    }
    switch (op.bits(Masks::b3210)) {
    case 0x01: pop_r16stk();
        break;
    case 0x05: push_r16stk();
        break;
    }
    if (op.bits(Masks::b210) == 0x03) rst_tgt3();
}

void CPU::blockCB() {
    fetch_opcode();
    switch (op.bits(Masks::b76)) {
    case 0x00:
        switch (op.bits(Masks::b543)) {
        case 0x00: rlc_r8();
            break;
        case 0x01: rrc_r8();
            break;
        case 0x02: rl_r8();
            break;
        case 0x03: rl_r8();
            break;
        case 0x04: sla_r8();
            break;
        case 0x05: sra_r8();
            break;
        case 0x06: swap_r8();
            break;
        case 0x07: srl_r8();
            break;
        }
        break;
    case 0x01: bit_b3_r8();
        break;
    case 0x10: res_b3_r8();
        break;
    case 0x11: set_b3_r8();
        break;
    }
}