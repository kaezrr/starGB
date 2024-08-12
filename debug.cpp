#include "debug.hpp"
#include "mnemonic.hpp"

// Print disassembly
void Debugger::write_text_log(const CPU& sm83, const Memory& mem) {
    file << std::format("[{:05d}] [{:04x}] [{:02x} {:02x} {:02x}] ", (int)sm83.elapsed_cycles, (int)sm83.PC.full,
        (int)mem.read(sm83.PC.full), (int)mem.read(sm83.PC.full + 1), (int)mem.read(sm83.PC.full + 2));
    file << get_op(mem.read(sm83.PC.full)) << '\n';

    if (sm83.PC.full == 0xCB) {
    file << std::format("[{:05d}] [{:04x}] [{:02x} {:02x} {:02x}] ", (int)sm83.elapsed_cycles, (int)sm83.PC.full + 1,
        (int)mem.read(sm83.PC.full+1), (int)mem.read(sm83.PC.full + 2), (int)mem.read(sm83.PC.full + 3));
    file << get_cb(mem.read(sm83.PC.full + 1)) << '\n';
    }

    print_cpu_mem(sm83, mem, file);
}

// Print cpu state, important registers
void Debugger::print_cpu_mem(const CPU& sm83, const Memory& mem, std::ostream& out) {
    out << std::format( "[AF: ${:04x} BC: ${:04x} DE: ${:04x} ",
        (int)sm83.AF.full, (int)sm83.BC.full, (int)sm83.DE.full);
    out << std::format("HL: ${:04x} SP: ${:04x} PC: ${:04x}\nLY: ${:02x} STAT: ${:02x} ",
        (int)sm83.HL.full, (int)sm83.SP.full, (int)sm83.PC.full, (int)mem.read(LY), (int) mem.read(STAT));
    out << std::format("IE: ${:02x} IF: ${:02x}]\n\n", (int)mem.read(IE), (int)mem.read(IF));
}

void Debugger::write_match_log(const CPU& sm83, [[maybe_unused]]const Memory& mem) {
    string str = std::format("a:{:02x} ", sm83.AF.hi) + "f:";
    str += (sm83.get_flag(Z) ? 'z' : '-');
    str += (sm83.get_flag(N) ? 'n' : '-');
    str += (sm83.get_flag(H) ? 'h' : '-');
    str += (sm83.get_flag(C) ? 'c' : '-');

    str += std::format(" bc:{:04x} de:{:04x} hl:{:04x} sp:{:04x} pc:{:04x} (cy: {:d})\n",
        sm83.BC.full, sm83.DE.full, sm83.HL.full, sm83.SP.full, sm83.PC.full, sm83.debug_cycles);
    file << str;
}

// Dump memory to a text file
void Debugger::memory_dump(const Memory& mem, u16 start, u16 end) {
    for (u16 i = start; i <= end; i += 0x10) {
        file << std::format("{:04x}:", i);
        for (u16 j = i; j < i + 16; j++) {
            file << std::format(" {:02x}", mem.read(j));
        }
        file << '\n';
    }
}

