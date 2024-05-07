#pragma once
#include <vector>
#include <ostream>
#include <tuple>
#include <string>

using std::vector, std::string, std::tuple;

enum class Log{
    read,
    write,
    null,
};


struct Debugger {
    vector<tuple<int, int, string>> logs;

    void clear_logs();
    void print_log(const tuple<int, int, string>& log, std::ostream& out);
    void print_all_logs(std::ostream& out);
    void log(Log log=Log::null, int address=0, int data=0);
};