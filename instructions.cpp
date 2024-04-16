#include "sm83.hpp"

// Instructions
void CPU::nop() {
}

void CPU::stop() {
}

void CPU::halt() {
}

void CPU::rlca() {
    set_flag(Z, false);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, AF.hi & 0x80);
    AF.hi = (AF.hi << 1) + AF.hi & 0x80;
}

void CPU::rrca() {
    set_flag(Z, false);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, AF.hi & 0x01);
    AF.hi = (AF.hi >> 1) + ((AF.hi & 0x01) << 7);
}

void CPU::rla() {
    u8 prev_carry = get_flag(C);
    set_flag(Z, false);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, AF.hi & 0x80);
    AF.hi = (AF.hi << 1) + prev_carry;
}

void CPU::rra() {
    u8 prev_carry = get_flag(C);
    set_flag(Z, false);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, AF.hi & 0x01);
    AF.hi = (AF.hi >> 1) + (prev_carry << 7);
}

void CPU::daa() {
    u8 correction = 0;
    bool subtract = get_flag(N);
    if (get_flag(H) || (!subtract && (AF.hi & 0xF) > 0x9)) {
        correction |= 0x06;
    }
    if (get_flag(C) || (!subtract && AF.hi > 0x99)) {
        correction |= 0x60;
        set_flag(C, true);
    }
    AF.hi += subtract ? -correction : correction;
    set_flag(Z, AF.hi == 0);
    set_flag(H, false);
}

void CPU::cpl() {
    AF.hi = ~AF.hi;
    set_flag(N, true);
    set_flag(H, true);
}

void CPU::scf() {
    set_flag(C, true);
    set_flag(N, false);
    set_flag(H, false);
}

void CPU::ccf() {
    set_flag(C, !get_flag(C));
    set_flag(N, false);
    set_flag(H, false);
}

void CPU::jr_imm8() {
    auto imm8 = static_cast<s8>(get_nextu8());
    PC.full += imm8;
}

void CPU::ld_r16_imm16() {
    write_r16(op.bits(Masks::b54), get_nextu16(), false);
}

void CPU::ld_r16mem_a() {
    write_r16mem(op.bits(Masks::b54), AF.hi);
}

void CPU::ld_a_r16mem() {
    AF.hi = read_r16mem(op.bits(Masks::b54));
}

void CPU::ld_imm16_sp() {
    u16 addr = get_nextu16();
    write_mem(addr, SP.full & 0xFF);
    write_mem(addr + 1, SP.full >> 8);
}

void CPU::inc_r16() {
    write_r16(op.bits(Masks::b54), read_r16(op.bits(Masks::b54), false) + 1, false);
}

void CPU::dec_r16() {
    write_r16(op.bits(Masks::b54), read_r16(op.bits(Masks::b54), false) - 1, false);
}

void CPU::add_hl_r16() {
    u16 reg = read_r16(op.bits(Masks::b54), false);
    u16 res = HL.full + reg;
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(HL.full, reg, res, Masks::hf16));
    set_flag(C, CFLAG_ADD(HL.full, reg, res, Masks::cf16));
    HL.full = res;
}

void CPU::inc_r8() {
    u8 reg = read_r8(op.bits(Masks::b543));
    u8 res = reg + 1;
    set_flag(Z, !reg);
    set_flag(N, CFLAG_ADD(reg, 1, res, Masks::hf8));
    set_flag(H, CFLAG_ADD(reg, 1, res, Masks::cf8));
    write_r8(op.bits(Masks::b543), res);
}

void CPU::dec_r8() {
    u8 reg = read_r8(op.bits(Masks::b543));
    u8 res = reg - 1;
    set_flag(Z, !reg);
    set_flag(N, CFLAG_SUB(reg, 1, res, Masks::hf8));
    set_flag(H, CFLAG_SUB(reg, 1, res, Masks::cf8));
    write_r8(op.bits(Masks::b543), res);
}

void CPU::ld_r8_imm8() {
    write_r8(op.bits(Masks::b543), get_nextu8());
}

void CPU::jr_cond_imm8() {
    auto imm8 = static_cast<s8>(get_nextu8());
    if (!check_cond(op.bits(Masks::b43))) return;
    PC.full += imm8;
}

void CPU::ld_r8_r8() {
    write_r8(op.bits(Masks::b543), read_r8(op.bits(Masks::b210)));
}

void CPU::add_a_r8() {
    auto r8 = read_r8(op.bits(Masks::b210));
    u32 res = AF.hi + r8;
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_ADD(AF.hi, r8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::adc_a_r8() {
    auto r8 = read_r8(op.bits(Masks::b210));
    u32 res = AF.hi + r8 + get_flag(C);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_ADD(AF.hi, r8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::sub_a_r8() {
    auto r8 = read_r8(op.bits(Masks::b210));
    u32 res = AF.hi - r8;
    set_flag(Z, !res);
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_SUB(AF.hi, r8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::sbc_a_r8() {
    auto r8 = read_r8(op.bits(Masks::b210));
    u32 res = AF.hi - r8 - get_flag(C);
    set_flag(Z, !res);
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_SUB(AF.hi, r8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::and_a_r8() {
}

void CPU::xor_a_r8() {
}

void CPU::or_a_r8() {
}

void CPU::cp_a_r8() {
}



