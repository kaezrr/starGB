#include "mbc0.hpp"
#include <fstream>

MBC0::MBC0(const string& path) {
    std::ifstream game{path};
    game.seekg(0x147, std::ios::beg);

    u8 type, romsz, ramsz;
    game >> type >> romsz >> ramsz;
	game.seekg(0, std::ios::beg);

    save_flag = (type == 0x9);
	game.read(reinterpret_cast<char*>(&rom_bank[0]), rom_bank.size());
    game.close();
    save_path = path.substr(0, path.find_last_of('.')) + ".sav";
    if(!save_flag)  return;
    std::ifstream save_file{save_path};
    if (!save_file) return;
    save_file.read(reinterpret_cast<char *>(&ram_bank[0]), ram_bank.size());
    save_file.close();
}

u8 MBC0::read_rom(u16 at) const {
    return rom_bank[at];
}

u8 MBC0::read_ram(u16 at) const {
    return ram_bank[at - EXRAM_S];
}

void MBC0::write_rom(u16 at, u8 data) {
    return;
}

void MBC0::write_ram(u16 at, u8 data) {
    ram_bank[at - EXRAM_S] = data;
}

void MBC0::save_ram() {
    std::ofstream save_file{save_path};
    save_file.write(reinterpret_cast<char *>(&ram_bank[0]), ram_bank.size());
    save_file.close();
}