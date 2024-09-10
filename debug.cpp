#include "debug.hpp"
#include "mnemonic.hpp"

void Debugger::log_path(const string& path) {
    file = std::ofstream{ path };
}

// Print disassembly
void Debugger::write_text_log() {
    file << std::format("[{:05d}] [{:04x}] [{:02x} {:02x} {:02x}] ", sm83->elapsed_cycles, sm83->PC.full,
        mem->read(sm83->PC.full), mem->read(sm83->PC.full + 1), mem->read(sm83->PC.full + 2));
    file << get_op(mem->read(sm83->PC.full)) << '\n';

    if (sm83->PC.full == 0xCB) {
    file << std::format("[{:05d}] [{:04x}] [{:02x} {:02x} {:02x}] ", sm83->elapsed_cycles, sm83->PC.full + 1,
        mem->read(sm83->PC.full+1), mem->read(sm83->PC.full + 2), mem->read(sm83->PC.full + 3));
    file << get_cb(mem->read(sm83->PC.full + 1)) << '\n';
    }

    print_cpu_mem(file);
}

// Print cpu state, important registers
void Debugger::print_cpu_mem(std::ostream& out) {
    out << std::format( "[AF: ${:04x} BC: ${:04x} DE: ${:04x} ",
        sm83->AF.full, sm83->BC.full, sm83->DE.full);
    out << std::format("HL: ${:04x} SP: ${:04x} PC: ${:04x}\nLY: ${:02x} STAT: ${:02x} ",
        sm83->HL.full, sm83->SP.full, sm83->PC.full, mem->read(LY),  mem->read(STAT));
    out << std::format("IE: ${:02x} IF: ${:02x}]\n\n", mem->read(IE), mem->read(IF));
}

void Debugger::write_match_log() {
    string str = std::format("a:{:02x} ", sm83->AF.hi) + "f:";
    str += (sm83->get_flag(Z) ? 'z' : '-');
    str += (sm83->get_flag(N) ? 'n' : '-');
    str += (sm83->get_flag(H) ? 'h' : '-');
    str += (sm83->get_flag(C) ? 'c' : '-');

    str += std::format(" bc:{:04x} de:{:04x} hl:{:04x} sp:{:04x} pc:{:04x} (cy: {:d})\n",
        sm83->BC.full, sm83->DE.full, sm83->HL.full, sm83->SP.full, sm83->PC.full, sm83->debug_cycles);
    file << str;
}

// Dump memory to a text file
void Debugger::memory_dump(u16 start, u16 end) {
    for (u16 i = start; i <= end; i += 0x10) {
        file << std::format("{:04x}:", i);
        for (u16 j = i; j < i + 16; j++) {
            file << std::format(" {:02x}", mem->read(j));
        }
        file << '\n';
    }
}

void Debugger::render_tiles() {
    for (u16 i = 0; i < 384; i++)
        fill_tile_data(i);

    return tiles.render_frame(tile_buffer.data());
}

void Debugger::fill_tile_data(u16 index) {
    int x = index % 16;
    int y = (index / 16) * 8;
    int addr = 0x8000 + (index * 16);

    for (int k = 0; k < 16; k += 2, y++) {
        u8 lsb = mem->vram[addr - VRAM_S + k];
        u8 msb = mem->vram[addr - VRAM_S + k + 1];

        for (u8 i = 0; i < 8; ++i) {
            u8 pixel = (((msb & (1 << i)) >> i) << 1) | ((lsb & (1 << i)) >> i);
            tile_buffer[(y * 128) + (x * 8) + (7 - i)] = pixel;
        }
    }
}
