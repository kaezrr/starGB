#include "timer.hpp"
#include<iostream>
#include<format>

Timer::Timer(Memory* mem) : memory{ mem } {
    memory->io_reg[TIMA - IO_S] = 0;
    memory->io_reg[TMA - IO_S] = 0;
    memory->io_reg[TAC - IO_S] = 0;
    memory->sys_clock = 0;
    memory->cycles_til_tima_irq = 0;
    memory->tima_reload_cycle = false;
    memory->last_edge = false;
    memory->tima_reload_cycle = false;
}

void Timer::req_timer_intr() {
    memory->io_reg[IF - IO_S] |= TIMER;
}

void Timer::tick() {
    std::cout << std::format("SCLK:{:016b} TIMA:{:02x} TMA:{:02x} TAC:{:02x}\n",
        memory->sys_clock, memory->io_reg[TIMA - IO_S], memory->io_reg[TMA - IO_S], memory->io_reg[TAC - IO_S]);

    memory->tima_reload_cycle = false;
    if (memory->cycles_til_tima_irq > 0) {
        memory->cycles_til_tima_irq--;
        if (memory->cycles_til_tima_irq == 0) {
            req_timer_intr();
            memory->io_reg[TIMA - IO_S] = memory->io_reg[TMA - IO_S];
            memory->tima_reload_cycle = true;
        }
    }
    memory->sys_clock_change(memory->sys_clock + 4);
}



