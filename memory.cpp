#include "memory.hpp"

u8 Memory::read(u16 at) const {
    if (at == SB)
        return 0xFF;

    if (at < 0x100 && execute_boot)
        return boot_rom[at];

    if (at >= ROM_S && at <= ROM_E) 	    
        return rom_banks[at - ROM_S];

    if (at >= VRAM_S && at <= VRAM_E) 	
        return vram[at - VRAM_S];

    if (at >= EXRAM_S && at <= EXRAM_E) 
        return exram[at - EXRAM_S];

    if (at >= WRAM_S && at <= ECHO_E) 	
        return wram[at & 0x1FFF];

    if (at >= OAM_S && at <= OAM_E) 	
        return oam[at - OAM_S];

    if (at >= FORBID_S && at <= FORBID_E)
        return (oam_lock) ? 0xFF : 0x00;

    if (at >= IO_S && at <= IO_E) {
        if (at == DIV) 
            return (sys_clock >> 8); // DIV register is the upper 8 bits of the system clock

        if (at == JOYP) {
            u8 reg = io_reg[at - IO_S] & 0xF0;
            u8 select = (~(input_buffer >> 4)) & 0xF ;
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

        return io_reg[at - IO_S];
    }

    if (at >= HRAM_S && at <= HRAM_E)
        return hram[at - HRAM_S];
    return ie_reg;
}


void Memory::write(u16 at, u8 data) {
    if (at == DMA) {
        u16 src = data << 8;
        for (u16 i = 0; i < 0xA0; ++i) {
            oam[i] = read(src + i);
        }
    }

    if (execute_boot && at == 0xFF50)
        execute_boot = false;

    if (tima_watch && !tima_write) 
        tima_write = (at == TIMA);

    else if (at >= VRAM_S && at <= VRAM_E) 	
        vram[at - VRAM_S] = data;

    else if (at >= EXRAM_S && at <= EXRAM_E) 
        exram[at - EXRAM_S] = data;

    else if (at >= WRAM_S && at <= WRAM_E) 	
        wram[at - WRAM_S] = data;

    else if (at >= ECHO_S && at <= ECHO_E) 	
        wram[at - ECHO_S] = data;

    else if (at >= OAM_S && at <= OAM_E) 	
        oam[at - OAM_S] = data;

    else if (at >= FORBID_S && at <= FORBID_E)
        return;

    else if (at >= IO_S && at <= IO_E) {
        if (at == DIV) {
            sys_clock = 0; // writing to system clock resets it

        } else if (at == STAT) {
            update_read_only(io_reg[at - IO_S], data | 0x80, 0x07);

        } else {
            io_reg[at - IO_S] = data;
        }
    }

    else if (at >= HRAM_S && at <= HRAM_E) 	
        hram[at - HRAM_S] = data;

    else if (at == IE_REG) 	
        ie_reg = data;
}

void Memory::reset() {
#define reset(arr) std::fill(arr.begin(), arr.end(), static_cast<u8>(0));
    reset(rom_banks);
    reset(boot_rom);
    reset(exram);
    reset(wram);
    reset(oam);
    reset(io_reg);
    reset(hram);
    ie_reg = 0;
    oam_lock = 0;
    tima_watch = 0;
    tima_write = 0;
    execute_boot = 0;
}

// Updates original with data except the read-only bits specified by the mask
void Memory::update_read_only(u8& original, u8 data, u8 mask) {
    original = (original & mask) | (data & ~mask);
}

