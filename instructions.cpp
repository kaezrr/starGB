#include "sm83.hpp"
#include <iostream>
#include <format>

// Instructions
void CPU::nop() {
    return;
}

void CPU::stop() {
    return;
}

void CPU::halt() {
    halt_mode = true;
}

void CPU::rlca() {
    set_flag(Z, false);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, AF.hi & 0x80);
    AF.hi = (AF.hi << 1) + get_flag(C);
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
    tick_others();
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
    tick_others();
    write_r16(op.bits(Masks::b54), read_r16(op.bits(Masks::b54), false) + 1, false);
}

void CPU::dec_r16() {
    tick_others();
    write_r16(op.bits(Masks::b54), read_r16(op.bits(Masks::b54), false) - 1, false);
}

void CPU::add_hl_r16() {
    u16 reg = read_r16(op.bits(Masks::b54), false);
    u16 res = HL.full + reg;
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(HL.full, reg, res, Masks::hf16));
    set_flag(C, CFLAG_ADD(HL.full, reg, res, Masks::cf16));
    tick_others();
    HL.full = res;
}

void CPU::inc_r8() {
    u8 reg = read_r8(op.bits(Masks::b543));
    u8 res = reg + 1;
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(reg, 1, res, Masks::hf8));
    write_r8(op.bits(Masks::b543), res);
}

void CPU::dec_r8() {
    u8 reg = read_r8(op.bits(Masks::b543));
    u8 res = reg - 1;
    set_flag(Z, !res);
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(reg, 1, res, Masks::hf8));
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
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_ADD(AF.hi, r8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::adc_a_r8() {
    auto r8 = read_r8(op.bits(Masks::b210));
    u32 res = AF.hi + r8 + get_flag(C);
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_ADD(AF.hi, r8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::sub_a_r8() {
    auto r8 = read_r8(op.bits(Masks::b210));
    u32 res = AF.hi - r8;
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_SUB(AF.hi, r8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::sbc_a_r8() {
    auto r8 = read_r8(op.bits(Masks::b210));
    u32 res = AF.hi - r8 - get_flag(C);
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_SUB(AF.hi, r8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::and_a_r8() {
    AF.hi &= read_r8(op.bits(Masks::b210));
    set_flag(Z, !AF.hi);
    set_flag(N, false);
    set_flag(H, true);
    set_flag(C, false);
}

void CPU::xor_a_r8() {
    AF.hi ^= read_r8(op.bits(Masks::b210));
    set_flag(Z, !AF.hi);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, false);
}

void CPU::or_a_r8() {
    AF.hi |= read_r8(op.bits(Masks::b210));
    set_flag(Z, !AF.hi);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, false);
}

void CPU::cp_a_r8() {
    auto r8 = read_r8(op.bits(Masks::b210));
    u32 res = AF.hi - r8;
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(AF.hi, r8, res, Masks::hf8));
    set_flag(C, CFLAG_SUB(AF.hi, r8, res, Masks::cf8));
}

void CPU::add_a_imm8(){
    auto imm8 = get_nextu8();
    u32 res = AF.hi + imm8;
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(AF.hi, imm8, res, Masks::hf8));
    set_flag(C, CFLAG_ADD(AF.hi, imm8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::adc_a_imm8(){
    auto imm8 = get_nextu8();
    u32 res = AF.hi + imm8 + get_flag(C);
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(AF.hi, imm8, res, Masks::hf8));
    set_flag(C, CFLAG_ADD(AF.hi, imm8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::sub_a_imm8(){
    auto imm8 = get_nextu8();
    u32 res = AF.hi - imm8;
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(AF.hi, imm8, res, Masks::hf8));
    set_flag(C, CFLAG_SUB(AF.hi, imm8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);

}

void CPU::sbc_a_imm8(){
    auto imm8 = get_nextu8();
    u32 res = AF.hi - imm8 - get_flag(C);
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(AF.hi, imm8, res, Masks::hf8));
    set_flag(C, CFLAG_SUB(AF.hi, imm8, res, Masks::cf8));
    AF.hi = static_cast<u8>(res);
}

void CPU::and_a_imm8(){
    AF.hi &= get_nextu8();
    set_flag(Z, !AF.hi);
    set_flag(N, false);
    set_flag(H, true);
    set_flag(C, false);
}

void CPU::xor_a_imm8(){
    AF.hi ^= get_nextu8();
    set_flag(Z, !AF.hi);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, false);

}

void CPU::or_a_imm8(){
    AF.hi |= get_nextu8();
    set_flag(Z, !AF.hi);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, false);
}

void CPU::cp_a_imm8(){
    auto imm8 = get_nextu8();
    u32 res = AF.hi - imm8;
    set_flag(Z, !static_cast<u8>(res));
    set_flag(N, true);
    set_flag(H, CFLAG_SUB(AF.hi, imm8, res, Masks::hf8));
    set_flag(C, CFLAG_SUB(AF.hi, imm8, res, Masks::cf8));
}

void CPU::ldh_c_a(){
    write_mem(0xFF00 + BC.lo, AF.hi);
}

void CPU::ldh_imm8_a(){
    auto imm8 = get_nextu8();
    write_mem(0xFF00 + imm8, AF.hi);
}

void CPU::ld_imm16_a(){
    auto imm16 = get_nextu16();
    write_mem(imm16, AF.hi);
}

void CPU::ldh_a_c(){
    AF.hi = read_mem(0xFF00 + BC.lo);
}

void CPU::ldh_a_imm8() {
    auto imm8 = get_nextu8();
    AF.hi = read_mem(0xFF00 + imm8);
}

void CPU::ld_a_imm16(){
    auto imm16 = get_nextu16();
    AF.hi = read_mem(imm16);
}

void CPU::add_sp_imm8(){
    auto imm8 = static_cast<s8>(get_nextu8());
    u32 res = SP.full + imm8;
    set_flag(Z, false);
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(SP.full, imm8, res, Masks::hf8));
    set_flag(C, CFLAG_ADD(SP.full, imm8, res, Masks::cf8));
    tick_others();
    tick_others();
    SP.full = static_cast<u16>(res);
}

void CPU::ld_hl_sp_plus_imm8(){
    auto imm8 = static_cast<s8>(get_nextu8());
    tick_others();
    u32 res = SP.full + imm8;
    set_flag(Z, false);
    set_flag(N, false);
    set_flag(H, CFLAG_ADD(SP.full, imm8, res, Masks::hf8));
    set_flag(C, CFLAG_ADD(SP.full, imm8, res, Masks::cf8));
    HL.full = static_cast<u16>(res);
}

void CPU::ld_sp_hl(){
    tick_others();
    SP.full = HL.full;
}

void CPU::di(){
    IME = false;
    pending_ime = false;
}

void CPU::ei(){
    pending_ime = true;
}

void CPU::ret(){
    PC.lo = read_mem(SP.full++);
    PC.hi = read_mem(SP.full++);
    tick_others();
}

void CPU::reti(){
    PC.lo = read_mem(SP.full++);
    PC.hi = read_mem(SP.full++);
    tick_others();
    IME = true;
}

void CPU::jp_imm16(){
    auto imm16 = get_nextu16();
    tick_others();
    PC.full = imm16;
}

void CPU::jp_hl(){
    PC.full = HL.full;
}

void CPU::call_imm16(){
    auto imm16 = get_nextu16();
    tick_others();
    write_mem(--SP.full, PC.hi);
    write_mem(--SP.full, PC.lo);
    PC.full = imm16;
}

void CPU::ret_cond(){
    if (!check_cond(op.bits(Masks::b43))) { 
        tick_others(); return;
    }
    PC.lo = read_mem(SP.full++);
    PC.hi = read_mem(SP.full++);
    tick_others();
}

void CPU::jp_cond_imm16(){
    auto imm16 = get_nextu16();
    if (!check_cond(op.bits(Masks::b43))) return;
    PC.full = imm16;
}

void CPU::call_cond_imm16(){
    auto imm16 = get_nextu16();
    if (!check_cond(op.bits(Masks::b43))) return;
    write_mem(--SP.full, PC.hi);
    write_mem(--SP.full, PC.lo);
    PC.full = imm16;
}

void CPU::rst_tgt3(){
    tick_others();
    u8 tgt = op.bits(Masks::b543) << 3;
    write_mem(--SP.full, PC.hi);
    write_mem(--SP.full, PC.lo);
    PC.full = tgt;
}

void CPU::pop_r16stk(){
    u8 lsb = read_mem(SP.full++);
    u8 msb = read_mem(SP.full++);
    write_r16(op.bits(Masks::b54), (msb << 8) | lsb, true);
}

void CPU::push_r16stk(){
    u16 reg = read_r16(op.bits(Masks::b54), true);
    u8 msb = static_cast<u8>((reg & 0xFF00) >> 8);
    u8 lsb = static_cast<u8>((reg & 0x00FF) >> 0);
    tick_others();
    write_mem(--SP.full, msb);
    write_mem(--SP.full, lsb);
}

void CPU::rlc_r8(){
    auto reg = read_r8(op.bits(Masks::b210));
    u8 res = (reg << 1) + ((reg & 0x80) >> 7);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, reg & 0x80);
    write_r8(op.bits(Masks::b210), res);
}

void CPU::rrc_r8(){
    auto reg = read_r8(op.bits(Masks::b210));
    u8 res = (reg >> 1) + ((reg & 0x01) << 7);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, reg & 0x01);
    write_r8(op.bits(Masks::b210), res);
}

void CPU::rl_r8(){
    auto reg = read_r8(op.bits(Masks::b210));
    u8 res = (reg << 1) + get_flag(C);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, reg & 0x80);
    write_r8(op.bits(Masks::b210), res);
}

void CPU::rr_r8(){
    auto reg = read_r8(op.bits(Masks::b210));
    u8 res = (reg >> 1) + (get_flag(C) << 7);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, reg & 0x01);
    write_r8(op.bits(Masks::b210), res);
}

void CPU::sla_r8(){
    auto reg = read_r8(op.bits(Masks::b210));
    u8 res = (reg << 1);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, reg & 0x80);
    write_r8(op.bits(Masks::b210), res);
}

void CPU::sra_r8(){
    auto reg = read_r8(op.bits(Masks::b210));
    u8 res = (reg >> 1) | (reg & 0x80);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, reg & 0x01);
    write_r8(op.bits(Masks::b210), res);
}

void CPU::swap_r8(){
    auto reg = read_r8(op.bits(Masks::b210));
    u8 res = (reg >> 4) | (reg << 4);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, false);
    write_r8(op.bits(Masks::b210), res);
}

void CPU::srl_r8(){
    auto reg = read_r8(op.bits(Masks::b210));
    u8 res = (reg >> 1);
    set_flag(Z, !res);
    set_flag(N, false);
    set_flag(H, false);
    set_flag(C, reg & 0x01);
    write_r8(op.bits(Masks::b210), res);
}

void CPU::bit_b3_r8(){
    auto bit = op.bits(Masks::b543);
    auto reg = read_r8(op.bits(Masks::b210));
    set_flag(Z, !(reg & (1 << bit)));
    set_flag(N, false);
    set_flag(H, true);
}

void CPU::res_b3_r8(){
    auto bit = op.bits(Masks::b543);
    auto reg = read_r8(op.bits(Masks::b210));
    write_r8(op.bits(Masks::b210), reg & ~(1 << bit));
}

void CPU::set_b3_r8(){
    auto bit = op.bits(Masks::b543);
    auto reg = read_r8(op.bits(Masks::b210));
    write_r8(op.bits(Masks::b210), reg | (1 << bit));
}
