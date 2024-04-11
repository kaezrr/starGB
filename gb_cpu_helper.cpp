#include "gb_cpu.hpp"


u8 CPU::read_r8(u8 r) const {
    switch (r) {
    case 0: return BC.hi;
    case 1: return BC.lo;
    case 2: return DE.hi;
    case 3: return DE.lo;
    case 4: return HL.hi;
    case 5: return HL.lo;
    case 6: return memory.read(HL.full);
    case 7: return AF.hi;
    }
    return 0;
}

u16 CPU::read_r16(u8 r, bool stk) const {
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
    case 0: return memory.read(BC.full); 
    case 1: return memory.read(DE.full); 
    case 2: return memory.read(HL.full++); 
    case 3: return memory.read(HL.full--); 
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
    case 6: memory.write(HL.full, data); break;
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
    case 0: memory.write(BC.full, data); break;
    case 1: memory.write(DE.full, data); break;
    case 2: memory.write(HL.full++, data); break;
    case 3: memory.write(HL.full--, data); break;
    }
}

void CPU::fetch_opcode() {
    opcode.byte = get_nextu8();
}

u8 CPU::get_nextu8() {
    return memory.read(PC.full++);
}

u16 CPU::get_nextu16() {
    u16 lsb = memory.read(PC.full++);
    u16 msb = memory.read(PC.full++);
    return (msb << 8) | lsb;
}

bool CPU::get_flag(Flags::Flag flag) const {
    return AF.lo & flag;
}

void CPU::set_flag(Flags::Flag flag, bool on) {
    if (on) AF.lo |= flag;
    else    AF.lo &= ~flag;
}

bool CPU::check_cond(u8 r) const {
    switch (r) {
    case 0: return !get_flag(Flags::Z);
    case 1: return  get_flag(Flags::Z);
    case 2: return !get_flag(Flags::C);
    case 3: return  get_flag(Flags::C);
    }
    return false;
}