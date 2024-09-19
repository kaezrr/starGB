#include "sm83.hpp"
#include <iostream>

void CPU::decode_opcode() {
    if (halt_mode) return;
    switch (op.byte) { // check for invalid ops or CB prefixed instructions
    case 0xD3:
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
        return;
    case 0xCB: blockCB();
        return;
    }

    switch (op.bits(Masks::b76)) {
    case 0x00: block00();
        return;
    case 0x01: block01();
        return;
    case 0x02: block02();
        return;
    case 0x03: block03();
        return;
    }
}

void CPU::block00() {
    switch (op.byte) {
    case 0x00: nop();
        return;
    case 0x10: stop();
        return;
    case 0x07: rlca();
        return;
    case 0x0F: rrca();
        return;
    case 0x17: rla();
        return;
    case 0x1F: rra();
        return;
    case 0x27: daa();
        return;
    case 0x2F: cpl();
        return;
    case 0x37: scf();
        return;
    case 0x3F: ccf();
        return;
    case 0x18: jr_imm8();
        return;
    }
    if ((op.byte & 0xE7) == 0x20) return jr_cond_imm8();

    switch (op.bits(Masks::b3210)) {
    case 0x01: ld_r16_imm16();
        return;
    case 0x02: ld_r16mem_a();
        return;
    case 0x0A: ld_a_r16mem();
        return;
    case 0x08: ld_imm16_sp();
        return;
    case 0x03: inc_r16();
        return;
    case 0x0B: dec_r16();
        return;
    case 0x09: add_hl_r16();
        return;
    }

    switch (op.bits(Masks::b210)) {
    case 0x04: inc_r8();
        return;
    case 0x05: dec_r8();
        return;
    case 0x06: ld_r8_imm8();
        return;
    }
}

void CPU::block01() {
    switch (op.byte) {
    case 0x76: halt();
        return;
    default: ld_r8_r8();
        return;
    }
}

void CPU::block02() {
    switch (op.bits(Masks::b76543)) {
    case 0x10: add_a_r8();
        return;
    case 0x11: adc_a_r8();
        return;
    case 0x12: sub_a_r8();
        return;
    case 0x13: sbc_a_r8();
        return;
    case 0x14: and_a_r8();
        return;
    case 0x15: xor_a_r8();
        return;
    case 0x16: or_a_r8();
        return;
    case 0x17: cp_a_r8();
        return;
    }
}

void CPU::block03() {
    switch (op.byte) {
    case 0xC6: add_a_imm8();
        return;
    case 0xCE: adc_a_imm8();
        return;
    case 0xD6: sub_a_imm8();
        return;
    case 0xDE: sbc_a_imm8();
        return;
    case 0xE6: and_a_imm8();
        return;
    case 0xEE: xor_a_imm8();
        return;
    case 0xF6: or_a_imm8();
        return;
    case 0xFE: cp_a_imm8();
        return;
    case 0xE2: ldh_c_a();
        return;
    case 0xE0: ldh_imm8_a();
        return;
    case 0xEA: ld_imm16_a();
        return;
    case 0xF2: ldh_a_c();
        return;
    case 0xF0: ldh_a_imm8();
        return;
    case 0xFA: ld_a_imm16();
        return;
    case 0xE8: add_sp_imm8();
        return;
    case 0xF8: ld_hl_sp_plus_imm8();
        return;
    case 0xCD: call_imm16();
        return;
    case 0xF9: ld_sp_hl();
        return;
    case 0xC3: jp_imm16();
        return;
    case 0xE9: jp_hl();
        return;
    case 0xC9: ret();
        return;
    case 0xD9: reti();
        return;
    case 0xF3: di();
        return;
    case 0xFB: ei();
        return;
    }

    if (op.bits(Masks::b210) == 0x07) rst_tgt3();

    if (!(op.byte & 0x20)) {
        switch(op.bits(Masks::b210)) {
        case 0x00: ret_cond();
            return;
        case 0x02: jp_cond_imm16();
            return;
        case 0x04: call_cond_imm16();
            return;
        }
    }
    switch (op.bits(Masks::b3210)) {
    case 0x01: pop_r16stk();
        return;
    case 0x05: push_r16stk();
        return;
    }
}

void CPU::blockCB() {
    fetch_opcode();
    switch (op.bits(Masks::b76)) {
    case 0x00:
        switch (op.bits(Masks::b543)) {
        case 0x00: rlc_r8();
            return;
        case 0x01: rrc_r8();
            return;
        case 0x02: rl_r8();
            return;
        case 0x03: rr_r8();
            return;
        case 0x04: sla_r8();
            return;
        case 0x05: sra_r8();
            return;
        case 0x06: swap_r8();
            return;
        case 0x07: srl_r8();
            return;
        }
        return;
    case 0x01: bit_b3_r8();
        return;
    case 0x02: res_b3_r8();
        return;
    case 0x03: set_b3_r8();
        return;
    }
}