#include "mbc3.hpp"
#include <fstream>

MBC3::MBC3(const string& path) {
    std::ifstream game{path};
    game.seekg(0x147, std::ios::beg);

    u8 type, romsz, ramsz;
    game >> type >> romsz >> ramsz;
	game.seekg(0, std::ios::beg);

    save_flag = (type == 0x10 || type == 0x13);
    rom_banks = vector<u8>(0x4000 * (1 << (romsz + 1)));
	game.read(reinterpret_cast<char*>(&rom_banks[0]), rom_banks.size());
    game.close();
    switch(ramsz) {
        case 0:
        case 1:
        case 2:
            ram_banks = vector<u8>(0x2000 * 1);
            break;
        case 3:
            ram_banks = vector<u8>(0x2000 * 4);
            break;
        case 4:
            ram_banks = vector<u8>(0x2000 * 16);
            break;
        case 5:
            ram_banks = vector<u8>(0x2000 * 8);
            break;
    }
    if(!save_flag)  return;
    save_path = path.substr(0, path.find_last_of('.')) + ".sav";
    std::ifstream save_file{save_path};
    if (!save_file) return;
    save_file.read(reinterpret_cast<char *>(&ram_banks[0]), ram_banks.size());
    save_file.close();
}

u8 MBC3::read_rom(u16 at) const {
    if(at <= 0x3FFF) // rom bank 0
        return rom_banks[at];
    if(at <= 0x7FFF) {// rom bank ...
        size_t addr = 0x4000 * rom_num + (at - 0x4000);
        return rom_banks[addr % rom_banks.size()];
    }
    return 0xFF;
}

void MBC3::write_rom(u16 at, u8 data) {
    if(at <= 0x1FFF) // exram is enabled if lower nibble is 0xA
        exram_enable = ((data & 0xF) == 0xA);
    else if(at <= 0x3FFF) { // set first 7 bits of rom bank number
        rom_num = (data & 0x7F);
        if(!data) rom_num = 0x01;
    } else if(at <= 0x5FFF)  // set ram bank number
        ram_num = data & 0x0F;
}

u8 MBC3::read_ram(u16 at) const {
    if (!exram_enable || ram_num > 3) return 0xFF;
    size_t addr = 0x2000 * ram_num + (at - 0xA000);
    return ram_banks[addr % ram_banks.size()];
}

void MBC3::write_ram(u16 at, u8 data) {
    if(!exram_enable) return;
    size_t addr = 0x2000 * ram_num + (at - 0xA000);
    ram_banks[addr % ram_banks.size()] = data;
}

void MBC3::save_ram() {
    if(!save_flag) return;
    std::ofstream save_file{ save_path };
    save_file.write(reinterpret_cast<char *>(&ram_banks[0]), ram_banks.size());
    save_file.close();
}
