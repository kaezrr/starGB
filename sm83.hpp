#pragma once

#include "cpu_utils.hpp"

struct CPU {
    Opcode op{};
    u8 t_cycles{};
    bool hard_lock{};
    Registers AF{}, BC{}, DE{}, HL{}, SP{}, PC{};
    Memory& memory;

    CPU(Memory& mem) : memory{ mem } {};

    void fetch_opcode();
    void decode_opcode();

    u8 read_mem(u16 at);
    void write_mem(u16 at, u8 data);

    u8 read_r8(u8 r);
    u8 read_r16mem(u8 r);
    u16 read_r16(u8 r, bool stk);

    u8 get_nextu8();
    u16 get_nextu16();

    void write_r8(u8 r, u8 data);
    void write_r16mem(u8 r, u8 data);
    void write_r16(u8 r, u16 data, bool stk);

    bool check_cond(u8 r);
    bool get_flag(Flag flag);
    void set_flag(Flag flag, bool on);

    // Opcode decoding blocks 
    void block00();
    void block01();
    void block02();
    void block03();
    void blockCB();
    
    // Instructions
    void nop();
    void stop();
    void halt();

    void rlca();
    void rrca();
    void rla();
    void rra();
    void daa();
    void cpl();
    void scf();
    void ccf();

    void ld_r16_imm16();
    void ld_r16mem_a();
    void ld_a_r16mem();
    void ld_imm16_sp();
    void ld_r8_imm8();
    void ld_r8_r8();

    void inc_r16();
    void inc_r8();
    void dec_r16();
    void dec_r8();

    void jr_imm8();
    void jr_cond_imm8();

    void add_hl_r16();
    void add_a_r8();
    void adc_a_r8();
    void sub_a_r8();
    void sbc_a_r8();

    void and_a_r8();
    void xor_a_r8();
    void or_a_r8();
    void cp_a_r8();
};
