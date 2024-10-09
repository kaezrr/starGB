#pragma once
#include "constants.hpp"

class Timer {
    u16 last_edge{0}, sys_clock{0};
    u8 tima{}, tma{}, tac{};
    int cycles_til_tima_irq{0};
    bool tima_reload_cycle{false};

    void req_timer_intr();
    void sys_clock_change(u16 new_value);
    void detect_edge(u16 before, u16 after);

  public:
    u8 interrupt_flag = 0;

    void tick();
    u8 read(u16 at);
    void write(u16 at, u8 data);
};