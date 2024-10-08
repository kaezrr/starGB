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

void Timer::set_div(u8 data) { sys_clock_change(0); }

void Timer::set_tima(u8 data) {
    if (!tima_reload_cycle)
        tima = data;
    if (cycles_til_tima_irq == 1)
        cycles_til_tima_irq = 0;
}

void Timer::set_tma(u8 data) {
    if (tima_reload_cycle)
        tima = data;
    tma = data;
}

void Timer::set_tac(u8 data) {
    u16 old_edge = last_edge;
    last_edge &= (data & 4) >> 2;
    detect_edge(old_edge, last_edge);
    tac = data;
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
