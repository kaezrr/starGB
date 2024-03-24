#include "memory.hpp"


static constexpr u16 ROM_S 	    = 0x0000;
static constexpr u16 ROM_E 	    = 0x7FFF;

static constexpr u16 VRAM_S 	= 0x8000;
static constexpr u16 VRAM_E 	= 0x9FFF;

static constexpr u16 EXRAM_S 	= 0xA000;
static constexpr u16 EXRAM_E 	= 0xBFFF;

static constexpr u16 WRAM_S 	= 0xC000;
static constexpr u16 WRAM_E 	= 0xDFFF;

static constexpr u16 ECHO_S 	= 0xE000;
static constexpr u16 ECHO_E 	= 0xFDFF;

static constexpr u16 OAM_S 	    = 0xFE00;
static constexpr u16 OAM_E 	    = 0xFE9F;

static constexpr u16 FORBID_S 	= 0xFEA0;
static constexpr u16 FORBID_E 	= 0xFEFF;

static constexpr u16 IO_S 	    = 0xFF00;
static constexpr u16 IO_E 	    = 0xFF7F;

static constexpr u16 HRAM_S 	= 0xFF80;
static constexpr u16 HRAM_E 	= 0xFFFE;

static constexpr u16 IE_REG 	= 0xFFFF;


u8 Memory::read(u16 at) {
    if (at >= ROM_S && at <= ROM_E) 	    
        return rom_banks[at - ROM_S];
    
    if (at >= VRAM_S && at <= VRAM_E) 	
        return vram[at - VRAM_S];

    if (at >= EXRAM_S && at <= EXRAM_E) 
        return exram[at - EXRAM_S];
    
    if (at >= WRAM_S && at <= WRAM_E) 	
        return wram[at - WRAM_S];

    if (at >= ECHO_S && at <= ECHO_E) 	
        return wram[at - ECHO_S];

    if (at >= OAM_S && at <= OAM_E) 	
        return oam[at - OAM_S];

    if (at >= FORBID_S && at <= FORBID_E)
        return (oam_lock) ? 0xFF : 0x00;

    if (at >= IO_S && at <= IO_E) 	
        return io_reg[at - IO_S];

    if (at >= HRAM_S && at <= HRAM_E) 	
        return hram[at - HRAM_S];

    if (at == IE_REG) 	
        return ie_reg;
}


void Memory::write(u16 at, u8 data) {
    if (at >= ROM_S && at <= ROM_E) 	    
        rom_banks[at - ROM_S] = data;
    
    if (at >= VRAM_S && at <= VRAM_E) 	
        vram[at - VRAM_S] = data;

    if (at >= EXRAM_S && at <= EXRAM_E) 
        exram[at - EXRAM_S] = data;
    
    if (at >= WRAM_S && at <= WRAM_E) 	
        wram[at - WRAM_S] = data;

    if (at >= ECHO_S && at <= ECHO_E) 	
        wram[at - ECHO_S] = data;

    if (at >= OAM_S && at <= OAM_E) 	
        oam[at - OAM_S] = data;

    if (at >= FORBID_S && at <= FORBID_E)
        return;

    if (at >= IO_S && at <= IO_E) 	
        io_reg[at - IO_S] = data;

    if (at >= HRAM_S && at <= HRAM_E) 	
        hram[at - HRAM_S] = data;

    if (at == IE_REG) 	
        ie_reg = data;
}
