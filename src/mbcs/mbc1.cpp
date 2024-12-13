#include "mbc.hpp"
#include <fstream>

MBC1::MBC1(const string& path) {
    std::ifstream game{path};
    game.seekg(0x147, std::ios::beg);

    u8 type, romsz, ramsz;
    game >> type >> romsz >> ramsz;
    game.seekg(0, std::ios::beg);

    save_flag = (type == 0x03);
    rom_banks = vector<u8>(0x4000 * (1 << (romsz + 1)));
    game.read(reinterpret_cast<char*>(&rom_banks[0]), rom_banks.size());
    game.close();
    switch (ramsz) {
    case 0:
    case 1:
        ram_banks = vector<u8>(0x0800 * 1);
        break;
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
    if (!save_flag)
        return;
    save_path = path.substr(0, path.find_last_of('.')) + ".sav";
    std::ifstream save_file{save_path};
    if (!save_file)
        return;
    save_file.read(reinterpret_cast<char*>(&ram_banks[0]), ram_banks.size());
    save_file.close();
}

u8 MBC1::read_rom(u16 at) const {
    if (at <= 0x3FFF) { // rom bank 0
        if (!mode_flag)
            return rom_banks[at];
        u16 addr = 0;
        switch (rom_banks.size()) {
        case (0x4000 * 0x40):
            addr = (ram_num & 0x1) << 5;
            break;
        case (0x4000 * 0x80):
            addr = (ram_num & 0x3) << 5;
            break;
        }
        return rom_banks[0x4000 * addr + at];
    }

    if (at <= 0x7FFF) { // rom bank ...
        u16 addr = rom_num;
        switch (rom_banks.size()) {
        case (0x4000 * 0x40):
            addr = ((rom_num & 0x1F) & ~(0x20)) | ((ram_num & 0x1) << 5);
            break;
        case (0x4000 * 0x80):
            addr = ((rom_num & 0x1F) & ~(0x60)) | ((ram_num & 0x3) << 5);
            break;
        }
        return rom_banks[(0x4000 * addr + (at - 0x4000)) % rom_banks.size()];
    }
    return 0xFF;
}

void MBC1::write_rom(u16 at, u8 data) {
    if (at <= 0x1FFF) // exram is enabled if lower nibble is 0xA
        exram_enable = ((data & 0xF) == 0xA);
    else if (at <= 0x3FFF) { // set rom bank number
        rom_num = data & 0x1F ? data & 0x1F : 1;
    } else if (at <= 0x5FFF) // set ram bank number
        ram_num = data & 0x03;
    else if (at <= 0x7FFF)
        mode_flag = data & 0x01;
}

u8 MBC1::read_ram(u16 at) const {
    if (!exram_enable)
        return 0xFF;
    size_t addr = 0;
    switch (ram_banks.size()) {
    case (0x0800):
    case (0x2000):
        addr = (at - 0xA000) % ram_banks.size();
        break;
    default:
        if (mode_flag)
            addr = 0x2000 * ram_num + (at - 0xA000);
        else
            addr = (at - 0xA000);
    }
    return ram_banks[addr % ram_banks.size()];
}

void MBC1::write_ram(u16 at, u8 data) {
    if (!exram_enable)
        return;
    size_t addr = 0;
    switch (ram_banks.size()) {
    case (0x0800):
    case (0x2000):
        addr = (at - 0xA000) % ram_banks.size();
        break;
    default:
        if (mode_flag)
            addr = 0x2000 * ram_num + (at - 0xA000);
        else
            addr = (at - 0xA000);
    }
    ram_banks[addr % ram_banks.size()] = data;
}

void MBC1::save_ram() {
    if (!save_flag)
        return;
    std::ofstream save_file{save_path};
    save_file.write(reinterpret_cast<char*>(&ram_banks[0]), ram_banks.size());
    save_file.close();
}