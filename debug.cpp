#include "debug.hpp"

void Debugger::log(Log log, int address, int data) {
    switch (log) {
    case Log::read: logs.emplace_back(address, data, "read");
        break;
    case Log::write: logs.emplace_back(address, data, "write");
        break;
    case Log::null: logs.emplace_back(address, data, "null");
        break;
    }
}

void Debugger::print_log(const tuple<int, int, string>& log, std::ostream& out) {
    const auto& [a, d, s] = log;
    if (s == "null") out << "null";
    else out << "[" << a << ',' << d << ',' << s << "]";
}
void Debugger::print_all_logs(std::ostream& out) {
    for (const auto& log : logs) {
        print_log(log, out); out << '\n';
    }
}

void Debugger::clear_logs() {
    logs.clear();
}
