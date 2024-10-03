#include "mbc5.hpp"
#include <fstream>

MBC5::MBC5(const string& path) {
    std::ifstream game{path};
    game.seekg(0x147, std::ios::beg);

    u8 type, romsz, ramsz;
    game >> type >> romsz >> ramsz;
	game.seekg(0, std::ios::beg);

    save_flag = (type == 0x3);
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
    save_path = path.substr(0, path.find_last_of('.')) + ".sav";
    if(save_flag) {
        std::ifstream save_file{ save_path };
        if(!save_file) return;
        save_file.read(reinterpret_cast<char *>(&ram_banks[0]), ram_banks.size());
        save_file.close();
    }
}

u8 MBC5::read_rom(u16 at) const {
    if(at <= 0x3FFF)
        return rom_banks[at];
    if(at <= 0x7FFF)
        return rom_banks[0x4000 * rom_num + (at - 0x4000)];
    return 0xFF;
}

u8 MBC5::read_ram(u16 at) const {
    if (!exram_enable) return 0xFF;
    return ram_banks[0x2000 * ram_num + (at - 0xA000)];
}

void MBC5::write_rom(u16 at, u8 data) {
    if(at <= 0x1FFF && (data & 0xF) == 0xA)
        exram_enable = true;
    else if(at <= 0x2FFF)
        rom_num = (rom_num & 0xFF) | data;
    else if(at <= 0x3FFF)
        rom_num = rom_num & (((u16)data & 1) << 8); 
    else if(at <= 0x5FFF)
        ram_num = data;
}

void MBC5::write_ram(u16 at, u8 data) {
    if(!exram_enable) return;
    ram_banks[0x2000 * ram_num + (at - 0xA000)] = data;
}

void MBC5::save_ram() {
    std::ofstream save_file{ save_path };
    save_file.write(reinterpret_cast<char *>(&ram_banks[0]), ram_banks.size());
    save_file.close();
}
