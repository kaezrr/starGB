#include "mbc1.hpp"

MBC1::MBC1(u8 rom_size, u8 ram_size, bool save) {
    rom_banks = vector<array<u8, 0x4000>>(1 << (rom_size + 1));
    ram_banks = vector<array<u8, 0x2000>>((rom_size == 0x3) ? 4 : 1);
    save_flag = save;
}