#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <sstream>

using std::vector, std::string, std::tuple;


struct Debugger {
    vector<tuple<int, int, string>> logs;

    void log();
    void clear_logs();
    void print_log(const tuple<int, int, string>& log, std::ostream& out);
    void print_all_logs(std::ostream& out);
    void log(int address, int data, bool read);
};