#include "memory.hpp"
#include "constants.hpp"
#include <fstream>
#include <memory>
#include <spdlog/spdlog.h>

u8 Memory::read(u16 at) const {
    if (at < 0x100 && execute_boot)
        return boot_rom[at];
    if (at <= ROM_E) 	    
        return mbc->read_rom(at);
    if (at <= VRAM_E) 	
        return ppu->fetcher.vram[at - VRAM_S];
    if (at <= EXRAM_E) 
        return mbc->read_ram(at);
    if (at <= ECHO_E) 	
        return wram[at & 0x1FFF];
    if (at <= OAM_E) 	
        return ppu->fetcher.oam[at - OAM_S];
    if (at <= FORBID_E)
        return 0x00;
    if (at <= IO_E) 
        return read_IO(at);
    if (at <= HRAM_E)
        return hram[at - HRAM_S];
    return ie_reg;
}

void Memory::write(u16 at, u8 data) {
    if (at == DMA) 
        initiate_dma_transfer(data);

    if (execute_boot && at == 0xFF50) 
        execute_boot = false;

    if(at <= ROM_E)
        mbc->write_rom(at, data);

    else if (at <= VRAM_E)
        ppu->fetcher.vram[at - VRAM_S] = data;

    else if (at <= EXRAM_E)
        mbc->write_ram(at, data);

    else if (at <= WRAM_E)
        wram[at - WRAM_S] = data;

    else if (at <= ECHO_E)
        wram[at - ECHO_S] = data;

    else if (at <= OAM_E)
        ppu->fetcher.oam[at - OAM_S] = data;

    else if (at <= FORBID_E)
        return;

    else if (at <= IO_E)
        write_IO(at, data);

    else if (at <= HRAM_E)
        hram[at - HRAM_S] = data;

    else if (at == IE_REG)
        ie_reg = data;
}

u8 Memory::read_IO(u16 at) const {
    switch (at) {
    case SB:
        return 0xFF;
    case DIV ... TAC:
        return timer->read(at);
    case LCDC ... WX:
        return ppu->read(at);
    case JOYP: {
        u8 reg = io_reg[at - IO_S] & 0xF0;
        u8 select = (~(input_buffer >> 4)) & 0xF;
        u8 dpad = (~input_buffer) & 0xF;
        switch ((reg >> 4) & 3) {
        case 0:
            return reg | (select & dpad);
        case 1:
            return reg | (select);
        case 2:
            return reg | (dpad);
        case 3:
            return reg | 0xF;
        }
    }
    case IF:
        return (io_reg[IF - IO_S] & ~TIMER) | (timer->interrupt_flag);
    }
    return io_reg[at - IO_S];
}

void Memory::write_IO(u16 at, u8 data) {
    switch (at) {
    case DIV ... TAC:
        return timer->write(at, data);
    case LCDC ... WX:
        return ppu->write(at, data);
    case SC:
        if (data == 0x81) {
            data = 0x01;
            io_reg[IF - IO_S] |= SERIAL;
        }
    
    case IF:
        timer->interrupt_flag = data & TIMER;
    }
  
    io_reg[at - IO_S] = data;
}

void Memory::initiate_dma_transfer(u8 data) {
    u16 src = data << 8;
    if (src >= 0xE000) src -= 0x2000;
    for (u16 i = 0; i < 0xA0; ++i) {
        ppu->fetcher.oam[i] = read(src + i);
    }
}

void Memory::load_boot(const string& path) {
	std::ifstream program{ path, std::ios::binary };
	if (!program) {
		spdlog::error("boot rom not found!");
        program.close();
		std::exit(1);
	}
	program.read(reinterpret_cast<char*>(&boot_rom[0]), 0x100);
	execute_boot = true;
}

void Memory::load_game(const string& path) {
	std::ifstream program{ path, std::ios::binary };
	if (!program) {
		spdlog::error("game rom not found!");
        program.close();
		std::exit(1);
	}
	program.seekg(0x147, std::ios::beg);
    u8 mbc_type; program >> mbc_type;
    program.close();

    switch(mbc_type) {
        case 0x00: 
        case 0x08:
        case 0x09:
            mbc = std::make_unique<MBC0>(path);
            return;
        case 0x01 ... 0x03: 
            mbc = std::make_unique<MBC1>(path);
            return;
        case 0x0F ... 0x13: 
            mbc = std::make_unique<MBC3>(path);
            return;
        case 0x19 ... 0x1E: 
            mbc = std::make_unique<MBC5>(path);
            return;
        default:
            spdlog::error("Unsupported MBC type! Emulator currently only supports MBC1, MBC3 and MC5");
            exit(1);
    }
}