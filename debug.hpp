#pragma once
#include <iostream>
#include <ostream>
#include <string>
#include <format>
#include <fstream>
#include "callback.hpp"
#include "sm83.hpp"
#include "memory.hpp"

using std::vector, std::string, std::tuple;

struct Debugger {
    std::ofstream file;
    const Memory* mem{ nullptr };
    const CPU* sm83{ nullptr };

    Debugger(Memory* mptr, CPU* cptr) : mem{ mptr }, sm83{ cptr } {}

    void log_path(const string& path);
    void write_text_log();
    void write_match_log();
    void print_cpu_mem(std::ostream& out);
    void memory_dump(u16 start, u16 end);
};