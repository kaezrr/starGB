#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "debug.hpp"
#include "memory.hpp"
#include "mnemonic.hpp"

// Print disassembly
void Debugger::write_text_log() {
  logger->debug(fmt::format(
      "[{:05d}] [{:04x}] [{:02x} {:02x} {:02x}] {}", sm83->elapsed_cycles,
      sm83->PC.full, mem->read(sm83->PC.full), mem->read(sm83->PC.full + 1),
      mem->read(sm83->PC.full + 2), get_op(mem->read(sm83->PC.full))));

  if (sm83->PC.full == 0xCB) {
    logger->debug(fmt::format(
        "[{:05d}] [{:04x}] [{:02x} {:02x} {:02x}] {}", sm83->elapsed_cycles,
        sm83->PC.full + 1, mem->read(sm83->PC.full + 1),
        mem->read(sm83->PC.full + 2), mem->read(sm83->PC.full + 3),
        get_cb(mem->read(sm83->PC.full + 1))));
    }

    logger->debug(fmt::format(
        "[AF: ${:04x} BC: ${:04x} DE: ${:04x} HL: ${:04x} SP: ${:04x} PC: ${:04x}\nLY: ${:02x} STAT: ${:02x} IE: ${:02x} IF: ${:02x}]\n",
        sm83->AF.full, sm83->BC.full, sm83->DE.full, sm83->HL.full,
        sm83->SP.full, sm83->PC.full, mem->read(LY), mem->read(STAT),
        mem->read(IE), mem->read(IF)));
}

// Dump memory to a text file
void Debugger::memory_dump(u16 start, u16 end) {
    auto memory_dump = spdlog::basic_logger_mt("memory-dump", "../logs/dump.txt");
    string str = "";
    for (u16 i = start; i <= end; i += 0x10) {
        str += fmt::format("{:04x}:", i);
        for (u16 j = i; j < i + 16; j++) {
            str += fmt::format(" {:02x}", mem->read(j));
        }
        memory_dump->debug(str);
    }
    memory_dump->flush();
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
