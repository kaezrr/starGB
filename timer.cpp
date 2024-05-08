#include "timer.hpp"

Timer::Timer(Memory* mem) : memory{ mem } { update_timers(); }

void Timer::update_timers() {
    u8 control = memory->read(TAC);
    tima_enable = control & 0x04;
    curr_cycles = 0;

    switch (control & 0x03) {
    case 0: cycles_threshold = 256;
        break;
    case 1: cycles_threshold = 4;
        break;
    case 2: cycles_threshold = 16;
        break;
    case 3: cycles_threshold = 64;
        break;
    }
}

void Timer::tick() {
    if (++curr_cycles < cycles_threshold) return;
    curr_cycles = 0;

    u16 div = memory->read(DIV) + 1;
    memory->write(DIV, static_cast<u8>(div));

    if (!tima_enable) return;
    u16 tima = memory->read(TIMA) + 1;
    if (tima > 0xFF) {
        tima = memory->read(TMA); 
        auto flag = memory->read(IF);
        memory->write(IF, flag | TIMER); // Request timer interrupt
    }
    memory->write(TIMA, static_cast<u8>(tima));
}