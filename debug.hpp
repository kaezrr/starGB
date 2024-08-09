#pragma once
#include <vector>
#include <ostream>
#include <tuple>
#include <string>
#include <format>
#include <fstream>
#include <algorithm>
#include "sm83.hpp"
#include "memory.hpp"

using std::vector, std::string, std::tuple;

struct Debugger {
    std::ofstream file{ "log_dump.txt" };

    void write_text_log(const CPU& sm83, const Memory& mem);
    void print_cpu_mem(const CPU& sm83, const Memory& mem);
    void write_match_log(const CPU& sm83, const Memory& mem);
    void memory_dump(const Memory& mem, u16 start, u16 end);
};