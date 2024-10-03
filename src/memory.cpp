#include "memory.hpp"
#include "constants.hpp"
#include <fstream>
#include <spdlog/spdlog.h>

u8 Memory::read(u16 at) const {
    if (at < 0x100 && execute_boot)
        return boot_rom[at];
    if (at <= ROM_E) 	    
        return read_rom(at);
    if (at <= VRAM_E) 	
        return vram[at - VRAM_S];
    if (at <= EXRAM_E) 
        return read_ram(at);
    if (at <= ECHO_E) 	
        return wram[at & 0x1FFF];
    if (at <= OAM_E) 	
        return oam[at - OAM_S];
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
        write_rom(at, data);

    else if (at <= VRAM_E)
        vram[at - VRAM_S] = data;

    else if (at <= EXRAM_E)
        write_ram(at, data);

    else if (at <= WRAM_E)
        wram[at - WRAM_S] = data;

    else if (at <= ECHO_E)
        wram[at - ECHO_S] = data;

    else if (at <= OAM_E)
        oam[at - OAM_S] = data;

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
    case DIV:
        return (sys_clock >> 8); // DIV register is the upper 8 bits of the system clock
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
    }
    return io_reg[at - IO_S];
}

void Memory::write_IO(u16 at, u8 data) {
    switch (at) {
    case DIV:
        sys_clock_change(0);
        return;

    case TIMA:
        if (!tima_reload_cycle) io_reg[at - IO_S] = data;
        if (cycles_til_tima_irq == 1) cycles_til_tima_irq = 0;
        return;

    case TMA:
        if (tima_reload_cycle) io_reg[TIMA - IO_S] = data;
        io_reg[at - IO_S] = data;
        return;

    case TAC: {
        u16 old_edge = last_edge;
        last_edge &= (data & 4) >> 2;
        detect_edge(old_edge, last_edge);
        io_reg[at - IO_S] = data;
        return;
    }
    case STAT:
        update_read_only(io_reg[at - IO_S], data | 0x80, 0x07);
        return;
    
    case SC:
        if (data == 0x81) {
            data = 0x01;
            io_reg[IF - IO_S] |= SERIAL;
        }
    }
  
    io_reg[at - IO_S] = data;
}

void Memory::initiate_dma_transfer(u8 data) {
    u16 src = data << 8;
    if (src >= 0xE000) src -= 0x2000;
    for (u16 i = 0; i < 0xA0; ++i) {
        oam[i] = read(src + i);
    }
}

void Memory::detect_edge(u16 before, u16 after) {
    if (before == 1 && after == 0) {
        io_reg[TIMA - IO_S]++;
        if (io_reg[TIMA - IO_S] == 0) {
            cycles_til_tima_irq = 1;
        }
    }
}

void Memory::sys_clock_change(u16 new_value) {
    sys_clock = new_value;
    u16 new_edge{ 0 };
    switch (io_reg[TAC - IO_S] & 3) {
    case 0:
        new_edge = (sys_clock >> 9) & 1;
        break;
    case 3:
        new_edge = (sys_clock >> 7) & 1;
        break;
    case 2:
        new_edge = (sys_clock >> 5) & 1;
        break;
    case 1:
        new_edge = (sys_clock >> 3) & 1;
        break;
    }
    new_edge &= (io_reg[TAC - IO_S] >> 2);
    detect_edge(last_edge, new_edge);
    last_edge = new_edge;
}

void Memory::update_read_only(u8& original, u8 data, u8 mask) {
    original = (original & mask) | (data & ~mask);
}

u8 Memory::read_rom(u16 at) const {
    switch (curr_controller) {
        case MemBC0:
            return mbc0.read_rom(at);
        case MemBC1:
        case MemBC3:
        case MemBC5:
            return mbc5.read_rom(at);
    }
    return 0xFF;
}

u8 Memory::read_ram(u16 at) const {
    switch (curr_controller) {
        case MemBC0:
            return mbc0.read_ram(at);
        case MemBC1:
        case MemBC3:
        case MemBC5:
            return mbc5.read_ram(at);
    }
    return 0xFF;
}

void Memory::write_rom(u16 at, u8 data) {
    switch (curr_controller) {
        case MemBC0:
            mbc0.write_rom(at, data);
        case MemBC1:
        case MemBC3:
        case MemBC5:
            mbc5.write_rom(at, data);
    }
}

void Memory::write_ram(u16 at, u8 data) {
    switch (curr_controller) {
        case MemBC0:
            mbc0.write_ram(at, data);
        case MemBC1:
        case MemBC3:
        case MemBC5:
            mbc5.write_ram(at, data);
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
            curr_controller = MemBC0;
            mbc0 = MBC0{path};
            return;
        case 0x01: 
        case 0x02:
        case 0x03:
            spdlog::error("MBC1 not yet supported!");
            exit(1);
        case 0x0F: 
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            spdlog::error("MBC3 not yet supported!");
            exit(1);
        case 0x19: 
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
            curr_controller = MemBC5;
            mbc5 = MBC5{path};
            return;
        default:
            spdlog::error("Unsupported MBC type! Emulator currently only supports MBC1, MBC3 and MC5");
            exit(1);
    }
}

void Memory::save_game() {
    switch(curr_controller) {
        case MemBC0:
            return mbc0.save_ram();
        case MemBC1:
        case MemBC3:
        case MemBC5:
            return mbc5.save_ram();
    }
}