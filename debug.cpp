#include "debug.hpp"

void Debugger::log(int address, int data, bool read) {
    logs.emplace_back(address, data, read ? "r-m" : "-wm");
}

void Debugger::log() {
    auto& [addr, data, str] = logs.back();
    logs.emplace_back(addr, data, "---");
}

void Debugger::print_log(const tuple<int, int, string>& log, std::ostream& out) {
    const auto& [a, d, s] = log;
    out << "[" << a << ',' << d << ',' << s << "]";
}
void Debugger::print_all_logs(std::ostream& out) {
    for (const auto& log : logs) {
        print_log(log, out); out << '\n';
    }
}

void Debugger::clear_logs() {
    logs.clear();
}
