#include "timer.hpp"

Timer::Timer(Memory* mem) : memory{ mem } { }

void Timer::tick() {
    t_tick();
    t_tick();
    t_tick();
    t_tick();
}

void Timer::t_tick() {
    memory->sys_clock++;

    u8 control = memory->read(TAC);
    bool tima_enable = control & 0x04;
    u16 flag{};
    switch (control & 0x03) {
    case 0: flag = 9; break;
    case 1: flag = 3; break;
    case 2: flag = 5; break;
    case 3: flag = 7; break;
    }
    flag = (1 << flag);

    bool curr_result = (memory->sys_clock & flag) && tima_enable;
    if (pending_overflow) return overflow();

    if (prev_result && !curr_result) {
        u16 tima_inc = memory->read(TIMA) + 1;
        if (tima_inc > 0xFF) {
            pending_overflow = true;
            memory->tima_watch = true;
            memory->tima_write = false;
        }
        memory->write(TIMA, static_cast<u8>(tima_inc));
    }
    prev_result = curr_result;
}

void Timer::overflow() {
    if (++overflow_cycles <= 4) {
        if (!memory->tima_write) return;
        overflow_cycles = 0;
        pending_overflow = false;
        memory->tima_watch = false;
        memory->tima_write = false;
    } else {
        overflow_cycles = 0;
        pending_overflow = false;
        u8 flag = memory->read(IF);
        u16 tma = memory->read(TMA);
        memory->write(IF, flag & TIMER);
        memory->write(TIMA, static_cast<u8>(tma));
    }
}