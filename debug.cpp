#include "debug.hpp"
#include "mnemonic.hpp"

// Print disassembly
void Debugger::write_text_log(const CPU& sm83, const Memory& mem, bool term = false) {
    std::ostream& out = term ? std::cout : file;
    out << std::format("[{:05d}] [{:04x}] [{:02x} {:02x} {:02x}] ", (int)sm83.elapsed_cycles, (int)sm83.PC.full,
        (int)mem.read(sm83.PC.full), (int)mem.read(sm83.PC.full + 1), (int)mem.read(sm83.PC.full + 2));
    out << get_op(mem.read(sm83.PC.full)) << '\n';

    if (sm83.PC.full == 0xCB) {
    out << std::format("[{:05d}] [{:04x}] [{:02x} {:02x} {:02x}] ", (int)sm83.elapsed_cycles, (int)sm83.PC.full + 1,
        (int)mem.read(sm83.PC.full+1), (int)mem.read(sm83.PC.full + 2), (int)mem.read(sm83.PC.full + 3));
    out << get_cb(mem.read(sm83.PC.full + 1)) << '\n';
    }

    print_cpu_mem(sm83, mem, out);
}

// Print cpu state, important registers
void Debugger::print_cpu_mem(const CPU& sm83, const Memory& mem, std::ostream& out) {
    out << std::format( "[AF: ${:04x} BC: ${:04x} DE: ${:04x} ",
        (int)sm83.AF.full, (int)sm83.BC.full, (int)sm83.DE.full);
    out << std::format("HL: ${:04x} SP: ${:04x} PC: ${:04x}\nLY: ${:02x} STAT: ${:02x} ",
        (int)sm83.HL.full, (int)sm83.SP.full, (int)sm83.PC.full, (int)mem.read(LY), (int) mem.read(STAT));
    out << std::format("IE: ${:02x} IF: ${:02x}]\n\n", (int)mem.read(IE), (int)mem.read(IF));
}

// Something really fucked up happened here
void Debugger::write_match_log(const CPU& sm83, const Memory& mem) {
    auto str = std::format("A: {:02x} F: {:02x} B: {:02x} C: {:02x} D: {:02x} E: {:02x} ",
        (int)sm83.AF.hi, (int)sm83.AF.lo, (int)sm83.BC.hi, (int)sm83.BC.lo, (int)sm83.DE.hi, (int)sm83.DE.lo);
    str += std::format("H: {:02x} L: {:02x} SP: {:04x} PC: 00:{:04x} ",
        (int)sm83.HL.hi, (int)sm83.HL.lo, (int)sm83.SP.full, (int)sm83.PC.full);
    str += std::format("({:02x} {:02x} {:02x} {:02x})\n", mem.read(sm83.PC.full), (int)mem.read(sm83.PC.full + 1),
        (int)mem.read(sm83.PC.full + 2), (int)mem.read(sm83.PC.full + 3));

    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {return (char)std::toupper(c); });
    file << str;
}

// Dump memory to a text file
void Debugger::memory_dump(const Memory& mem, u16 start, u16 end) {
    std::ofstream memo{ "memory_dump.txt" };
    for (u16 i = start; i <= end; i += 0x10) {
        memo << std::format("{:04x}:", i);
        for (u16 j = i; j < i + 16; j++) {
            memo << std::format(" {:02x}", mem.read(j));
        }
        memo << '\n';
    }
}

