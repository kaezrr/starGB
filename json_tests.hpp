#pragma once
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "gameboy.hpp"

using json = nlohmann::json;

void print_error(std::ostream& out, const string& error_message, int expected, int got) {
    out << error_message << ", expected: [" << expected << "], got: [" << got << "]\n";
}

void load_cpu_data(GameBoy& gb, json& data) {
    gb.memory.reset();
    gb.debugger.clear_logs();

    gb.sm83.AF.hi   = static_cast<u8>(data["a"]);
    gb.sm83.AF.lo   = static_cast<u8>(data["f"]);

    gb.sm83.BC.hi   = static_cast<u8>(data["b"]);
    gb.sm83.BC.lo   = static_cast<u8>(data["c"]);

    gb.sm83.DE.hi   = static_cast<u8>(data["d"]);
    gb.sm83.DE.lo   = static_cast<u8>(data["e"]);

    gb.sm83.HL.hi   = static_cast<u8>(data["h"]);
    gb.sm83.HL.lo   = static_cast<u8>(data["l"]);

    gb.sm83.PC.full = static_cast<u16>(data["pc"]);
    gb.sm83.SP.full = static_cast<u16>(data["sp"]);

    for (auto& ram : data["ram"]) {
        auto addr = static_cast<u16>(ram[0]);
        auto byte = static_cast<u8>(ram[1]);
        gb.memory.write(addr, byte);
    }
}

bool match_cpu_data(GameBoy& gb, json& data, std::ostream& out) {
    if (gb.sm83.AF.hi != static_cast<u8>(data["a"])) {
        print_error(out, "A register mismatch", data["a"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.AF.lo != static_cast<u8>(data["f"])) {
        print_error(out, "F register mismatch", data["f"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.BC.hi != static_cast<u8>(data["b"])) {
        print_error(out, "B register mismatch", data["b"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.BC.lo != static_cast<u8>(data["c"])) {
        print_error(out, "C register mismatch", data["c"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.DE.hi != static_cast<u8>(data["d"])) {
        print_error(out, "D register mismatch", data["d"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.DE.lo != static_cast<u8>(data["e"])) {
        print_error(out, "E register mismatch", data["e"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.HL.hi != static_cast<u8>(data["h"])) {
        print_error(out, "H register mismatch", data["h"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.HL.lo != static_cast<u8>(data["l"])) {
        print_error(out, "L register mismatch", data["l"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.PC.full != static_cast<u16>(data["pc"])) {
        print_error(out, "program counter mismatch", data["pc"], gb.sm83.AF.hi);
        return false;
    }
    if (gb.sm83.SP.full != static_cast<u16>(data["sp"])) {
        print_error(out, "stack pointer mismatch", data["sp"], gb.sm83.AF.hi);
        return false;
    }

    for (auto& ram : data["ram"]) {
        auto addr = static_cast<u16>(ram[0]);
        auto byte = static_cast<u8>(ram[1]);
        if (gb.memory.read(addr) == byte) continue;
        out << "memory at location " << ram[0]
            << " is wrong, expected: ["<< ram[1]
            << "], got: [" << gb.memory.read(addr) << "]\n";
        return false;
    }

    return true;
}

bool run_test(json& data, GameBoy& gb, std::ostream& out) {
    load_cpu_data(gb, data["initial"]);
    gb.run_instruction();

    if (data["cycles"].size() != gb.debugger.logs.size()) {
        out << "wrong number of cycles, expected: [" 
            << data["cycles"].size() << "], got: [" 
            << gb.debugger.logs.size() << "]\n";
        return false;
    }

    int cycle = 0;
    for (const auto& c : data["cycles"]) {
        auto& [a, d, s] = gb.debugger.logs[cycle];
        if (c[0] == a && c[1] == d && c[2].get<string>() == s) { ++cycle; continue; }
        out << "cycle " << cycle << " is wrong, expected: " << c << ", got: ";
        gb.debugger.print_log(gb.debugger.logs[cycle], out); out << "\n";
        return false;
    }
    return match_cpu_data(gb, data["final"], out);
}

bool run_json_test(const std::string& file_path, GameBoy& gb, std::ostream& out) {
    std::ifstream file{ file_path };
    if (!file) { out << "File path: [" << file_path << "] not found!\n"; return false; }
    json data = json::parse(file);

    for (auto& element : data) {
        std::stringstream ss;
        if (run_test(element, gb, ss)) { continue; }
        out << "[" << element["name"].get<string>() 
            << "]: " << ss.str();
        return false;
    }
    return true;
}

void run_all_tests(const string& folder, GameBoy& gb, std::ostream& out) {
    int count = 0;
    for (int i = 0x00; i <= 0xff; ++i) {
        switch (i) { // Ignore invalid opcodes, cb, stop and halt.
        case 0xD3:
        case 0xDB:
        case 0xDD:
        case 0xE3:
        case 0xE4:
        case 0xEB:
        case 0xEC:
        case 0xED:
        case 0xF4:
        case 0xFC:
        case 0xFD:
        case 0xCB:
        case 0x10:
        case 0x76:
            continue;
        }
        ++count;
        std::stringstream ss; ss << std::hex << folder;
        if (i <= 0xf) ss << '0'; ss << i << ".json";
        if (!run_json_test(ss.str(), gb, out)) return;
        out << "Opcode: 0x" << std::hex << i << " passed!\n";
    }
    // cb prefix opcodes test
    for (int i = 0x00; i <= 0xff; ++i, ++count) {
        std::stringstream ss; ss << std::hex << folder << "cb ";
        if (i <= 0xf) ss << '0'; ss << i << ".json";
        if (!run_json_test(ss.str(), gb, out)) return;
        out << "Opcode: 0xcb " << std::hex << i << " passed!\n";
    }
    gb.memory.reset();
    gb.debugger.clear_logs();
    out << "Tested " << std::dec << count << " opcodes. [Ignored invalid opcodes, stop and halt]\n";
    out << "All tests passed! :)\n";
}

