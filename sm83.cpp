#include "sm83.hpp"

u8 CPU::read_mem(u16 at) {
    t_cycles += 4;
    return memory.read(at);
}

void CPU::write_mem(u16 at, u8 data) {
    t_cycles += 4;
    memory.write(at, data);
}

u8 CPU::read_r8(u8 r) {
    switch (r) {
    case 0: return BC.hi;
    case 1: return BC.lo;
    case 2: return DE.hi;
    case 3: return DE.lo;
    case 4: return HL.hi;
    case 5: return HL.lo;
    case 6: return read_mem(HL.full);
    case 7: return AF.hi;
    }
    return 0;
}

u16 CPU::read_r16(u8 r, bool stk) {
    switch (r) {
    case 0: return BC.full;
    case 1: return DE.full;
    case 2: return HL.full;
    case 3: 
        if (stk) return AF.full;
        return SP.full;
    }
    return 0;
}

u8 CPU::read_r16mem(u8 r) {
    switch (r) {
    case 0: return read_mem(BC.full); 
    case 1: return read_mem(DE.full); 
    case 2: return read_mem(HL.full++); 
    case 3: return read_mem(HL.full--); 
    }
    return 0;
}

void CPU::write_r8(u8 r, u8 data) {
    switch (r) {
    case 0: BC.hi = data; break;
    case 1: BC.lo = data; break;
    case 2: DE.hi = data; break;
    case 3: DE.lo = data; break;
    case 4: HL.hi = data; break;
    case 5: HL.lo = data; break;
    case 6: write_mem(HL.full, data); break;
    case 7: AF.hi = data; break;
    }
}

void CPU::write_r16(u8 r, u16 data, bool stk) {
    switch (r) {
    case 0: BC.full = data; break;
    case 1: DE.full = data; break;
    case 2: HL.full = data; break;
    case 3: 
        if (stk) AF.full = data;
        else SP.full = data; break;
    }
}

void CPU::write_r16mem(u8 r, u8 data) {
    switch (r) {
    case 0: write_mem(BC.full, data); break;
    case 1: write_mem(DE.full, data); break;
    case 2: write_mem(HL.full++, data); break;
    case 3: write_mem(HL.full--, data); break;
    }
}

void CPU::fetch_opcode() {
    t_cycles = 4;
    op.byte = get_nextu8();
}

u8 CPU::get_nextu8() {
    return read_mem(PC.full++);
}

u16 CPU::get_nextu16() {
    u16 lsb = read_mem(PC.full++);
    u16 msb = read_mem(PC.full++);
    return (msb << 8) | lsb;
}

bool CPU::get_flag(Flag flag) {
    return AF.lo & flag;
}

void CPU::set_flag(Flag flag, bool on) {
    if (on) AF.lo |= flag;
    else    AF.lo &= ~flag;
}

bool CPU::check_cond(u8 r) {
    bool cond{};
    switch (r) {
    case 0: cond = !get_flag(Z); break;
    case 1: cond =  get_flag(Z); break;
    case 2: cond = !get_flag(C); break;
    case 3: cond =  get_flag(C); break;
    }
    if (cond) t_cycles += 4;
    return cond;
}


