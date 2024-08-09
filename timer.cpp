#include "timer.hpp"
#include <iostream>
#include <format>

Timer::Timer(Memory* mem) : memory{ mem } { }

void Timer::tick() {
    memory->sys_clock += 4;
    u8 tac = memory->read(TAC);
    u16 flag{}; // select DIV bit to read from
    switch (tac & 0x03) {
    case 0: flag = 9; break;
    case 1: flag = 3; break;
    case 2: flag = 5; break;
    case 3: flag = 7; break;
    }
    flag = (1 << flag);

    bool curr_edge = (memory->sys_clock & flag) && (tac & 0x4);
    if (pending_overflow) { 
        prev_edge = curr_edge; 
        return overflow();
    }
    // increment TAC on falling edge
    if (prev_edge && !curr_edge) {
        u16 tima_inc = memory->io_reg[TIMA - IO_S] + 1;
        if (tima_inc > 0xFF) {
            pending_overflow = true;
            memory->tima_watch = true;
            memory->tima_write = false;
        }
        memory->io_reg[TIMA - IO_S] = static_cast<u8>(tima_inc);
    }
    prev_edge = curr_edge;
}

void Timer::overflow() {
    if (++overflow_cycles <= 1) {
        if (!memory->tima_write) return;
    } else {
        req_timer_intr();
        memory->io_reg[TIMA - IO_S] = memory->io_reg[TMA - IO_S];  
    }
    // Reset TIMA overflow stuff
    overflow_cycles = 0;
    pending_overflow = false;
    memory->tima_watch = false;
    memory->tima_write = false;
}

void Timer::req_timer_intr() {
    memory->io_reg[IF - IO_S] |= TIMER;
}