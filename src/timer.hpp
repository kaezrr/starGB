#pragma once
#include "constants.hpp"

class Timer {
    u16 last_edge{0};
    int cycles_til_tima_irq{0};
    bool tima_reload_cycle{false};

    void req_timer_intr();
    void sys_clock_change(u16 new_value);
    void detect_edge(u16 before, u16 after);

  public:
    u16 sys_clock{0};
    u8 tima{}, tma{}, tac{};
    u8 interrupt_flag = 0;

    void tick();
    void set_div(u8 data);
    void set_tima(u8 data);
    void set_tma(u8 data);
    void set_tac(u8 data);
};