#include "timer.hpp"

void Timer::req_timer_intr() {
    interrupt_flag |= TIMER;
}

void Timer::tick() {
    tima_reload_cycle = false;
    if (cycles_til_tima_irq > 0) {
        cycles_til_tima_irq--;
        if (cycles_til_tima_irq == 0) {
            req_timer_intr();
            tima = tma;
            tima_reload_cycle = true;
        }
    }
    sys_clock_change(sys_clock + 4);
}

void Timer::detect_edge(u16 before, u16 after) {
    if (before == 1 && after == 0) {
        tima++;
        if (tima == 0) {
            cycles_til_tima_irq = 1;
        }
    }
}

void Timer::sys_clock_change(u16 new_value) {
    sys_clock = new_value;
    u16 new_edge{ 0 };
    switch (tac & 3) {
    case 0:
        new_edge = (sys_clock >> 9) & 1;
        break;
    case 3:
        new_edge = (sys_clock >> 7) & 1;
        break;
    case 2:
        new_edge = (sys_clock >> 5) & 1;
        break;
    case 1:
        new_edge = (sys_clock >> 3) & 1;
        break;
    }
    new_edge &= (tac >> 2);
    detect_edge(last_edge, new_edge);
    last_edge = new_edge;
}

u8 Timer::read(u16 at) {
    switch(at) {
    case DIV:
        return sys_clock << 8;
    case TIMA:
        return tima;
    case TMA:
        return tma;
    case TAC:
        return tac;
    default:
        return 0xFF;
    }
}

void Timer::write(u16 at, u8 data) {
    switch(at) {
    case DIV:
        sys_clock_change(0);
        return;
    case TIMA:
        if (!tima_reload_cycle)
            tima = data;
        if (cycles_til_tima_irq == 1)
            cycles_til_tima_irq = 0;
        return;
    case TMA:
        if (tima_reload_cycle)
            tima = data;
        tma = data;
        return;
    case TAC: {
        last_edge &= (data & 4) >> 2;
        u16 old_edge = last_edge;
        detect_edge(old_edge, last_edge);
        tac = data;
        return;
    }
    }
}