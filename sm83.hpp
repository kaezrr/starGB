#pragma once
#include "cpu_utils.hpp"
#include "debug.hpp"
#include "timer.hpp"

struct CPU {
    Opcode op{};
    Memory* memory;
    Timer* timer;
    Debugger* debug;
    Registers AF{}, BC{}, DE{}, HL{}, SP{}, PC{};

    CPU(Memory* memory_ptr, Timer* timer_ptr, Debugger* debug_ptr = nullptr);

    // Flags
    bool hard_lock{};
    bool pending_ime{};
    bool IME{};

    void fetch_opcode();
    void decode_opcode();
    void tick_others(Log log=Log::null, u16 at=0, u8 data=0);

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

    void add_a_imm8();
    void adc_a_imm8();
    void sub_a_imm8();
    void sbc_a_imm8();
    void and_a_imm8();
    void xor_a_imm8();
    void or_a_imm8();
    void cp_a_imm8();

    void ldh_c_a();
    void ldh_imm8_a();
    void ld_imm16_a();
    void ldh_a_c();
    void ldh_a_imm8();
    void ld_a_imm16();
    void add_sp_imm8();
    void ld_hl_sp_plus_imm8();
    void ld_sp_hl();
    void di();
    void ei();

    void ret();
    void reti();
    void jp_imm16();
    void jp_hl();
    void call_imm16();

    void ret_cond();
    void jp_cond_imm16();
    void call_cond_imm16();
    void rst_tgt3();
    void pop_r16stk();
    void push_r16stk();

    void rlc_r8();
    void rrc_r8();
    void rl_r8();
    void rr_r8();
    void sla_r8();
    void sra_r8();
    void swap_r8();
    void srl_r8();

    void bit_b3_r8();
    void res_b3_r8();
    void set_b3_r8();
};
