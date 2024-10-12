#include<spdlog/spdlog.h>
#include "sm83.hpp"
#include "cpu_utils.hpp"

CPU::CPU(Memory* memory_ptr, PPU* ppu_ptr, Timer* timer_ptr)
    :  ppu{ ppu_ptr }, memory{ memory_ptr },timer{ timer_ptr } {};

void CPU::tick() {
    elapsed_cycles++;
    m_cycles++;
}

void CPU::cycle_parts(int cycles) {
    for (int _ = 0; _ < cycles; ++_) {
        timer->tick();
        ppu->tick();
    }
    m_cycles = 0;
}

u8 CPU::read_mem(u16 at) {
    tick();
    cycle_parts(m_cycles);
    auto data = memory->read(at);
    return data;
}

void CPU::write_mem(u16 at, u8 data) {
    tick();
    cycle_parts(m_cycles);
    memory->write(at, data);
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
        if (stk) AF.full = data & 0xFFF0;
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
    if (halt_mode) return tick();
    if (IME == false) { 
        IME = pending_ime; 
        pending_ime = false;
    }

    op.byte = get_nextu8();
}

u8 CPU::get_nextu8() {
    return read_mem(PC.full++);
}

u16 CPU::get_nextu16() {
    u16 lsb = get_nextu8();
    u16 msb = get_nextu8();
    return (msb << 8) | lsb;
}

bool CPU::get_flag(Flag flag) const {
    return AF.lo & flag;
}

void CPU::set_flag(Flag flag, bool on) {
    if (on) AF.lo |= flag;
    else    AF.lo &= ~flag;
}

bool CPU::check_cond(u8 r) {
    bool cond{};
    switch (r) {
    case 0: cond = !get_flag(Z);
        break;
    case 1: cond =  get_flag(Z); 
        break;
    case 2: cond = !get_flag(C); 
        break;
    case 3: cond =  get_flag(C); 
        break;
    }
    if (cond) tick();

    return cond;
}

void CPU::handle_interrupts() {
    u8 enable       = memory->read(IE);
    u8 flag         = memory->read(IF);
    u8 interrupts   = flag & enable;

    // check for interrupts
    if (interrupts) halt_mode = false;
    if (!interrupts || !IME) return;

    u8 priority_bit = (1 << countr_zero(interrupts));

    // disable further interrupt handling
    IME = false;
    memory->write(IF, flag & ~priority_bit);

    tick();
    tick();

    // push PC onto stack
    write_mem(--SP.full, PC.hi);
    write_mem(--SP.full, PC.lo);

    tick();
    switch (priority_bit) {
    case VBLANK:
        //spdlog::info("VBLANK");
        PC.full = 0x40; return;
    case LCD:
        //spdlog::info("LCD");
        PC.full = 0x48; return;
    case TIMER:
        //spdlog::info("TIMER");
        PC.full = 0x50; return;
    case SERIAL:
        //spdlog::debug("SERIAL");
        PC.full = 0x58; return;
    case JOYPAD:
        //spdlog::debug("JOYPAD");
        PC.full = 0x60; return;
    }
}


