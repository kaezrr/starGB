#include "debug.hpp"
#include "mnemonic.hpp"


void Debugger::write_text_log(const CPU& sm83, const Memory& mem) {
    file << std::format("[{:05d}] [{:04x}] [{:02x}, {:02x}, {:02x}] ", (int)sm83.elapsed_cycles, (int)sm83.PC.full,
        (int)mem.read(sm83.PC.full), (int)mem.read(sm83.PC.full + 1), (int)mem.read(sm83.PC.full + 2));
    file << get_op(mem.read(sm83.PC.full)) << '\n';

    if (sm83.PC.full == 0xCB) {
    file << std::format("[{:05d}] [{:04x}] [{:02x}, {:02x}, {:02x}] ", (int)sm83.elapsed_cycles, (int)sm83.PC.full + 1,
        (int)mem.read(sm83.PC.full+1), (int)mem.read(sm83.PC.full + 2), (int)mem.read(sm83.PC.full + 3));
    file << get_cb(mem.read(sm83.PC.full + 1)) << '\n';
    }

    print_cpu_mem(sm83, mem);
}

void Debugger::print_cpu_mem(const CPU& sm83, const Memory& mem) {
    file << std::format( "[A: ${:02x}, F: ${:02x}, B: ${:02x}, C: ${:02x}, D: ${:02x}, E: ${:02x}, ",
        (int)sm83.AF.lo, (int)sm83.AF.hi, (int)sm83.BC.lo, (int)sm83.BC.hi, (int)sm83.DE.lo, (int)sm83.DE.hi);
    file << std::format("H: ${:02x}, L: ${:02x}, SP: ${:04x}, PC: ${:04x}, LY: {:03d}]\n\n",
        (int)sm83.HL.lo, (int)sm83.HL.hi, (int)sm83.SP.full, (int)sm83.PC.full, (int)mem.read(LY));
}
