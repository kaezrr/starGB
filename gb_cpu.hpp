#pragma once

#include "gb_memory.hpp"
#include<bit>

union Registers {
    u16 full;
    struct{ u8 lo, hi; };
};

struct Opcode {
    u8 byte;
    inline u8 bits(u8 mask) const; 
    Opcode(u8 op) : byte{ op } {}
};

namespace Flags {
    enum Flag {
        Z = 0x80,
        N = 0x40,
        H = 0x20,
        C = 0x10,
    };
}

class CPU {
private:
    Opcode opcode{ 0 };
    bool hard_lock{};
    Registers AF{}, BC{}, DE{}, HL{}, SP{}, PC{};
    Memory& memory;

    u8 read_r8(u8 r) const;
    u8 read_r16mem(u8 r);
    u16 read_r16(u8 r, bool stk) const;

    u8 get_nextu8();
    u16 get_nextu16();

    void write_r8(u8 r, u8 data);
    void write_r16mem(u8 r, u8 data);
    void write_r16(u8 r, u16 data, bool stk);

    // Opcode decoder blocks
    u8 block0();
    u8 block1();
    u8 block2();
    u8 block3();
    u8 blockCB();

    bool check_cond(u8 r) const;
    bool get_flag(Flags::Flag flag) const;
    void set_flag(Flags::Flag flag, bool on);

public:
    CPU(Memory& mem) : memory{ mem } {};
    void fetch_opcode();
    u8 decode_opcode();
};