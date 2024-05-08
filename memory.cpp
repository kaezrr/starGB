#include "memory.hpp"

u8 Memory::read(u16 at) const {
#ifndef TEST
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

    if (at >= IO_S && at <= IO_E) 	
        return io_reg[at - IO_S];

    if (at >= HRAM_S && at <= HRAM_E)
        return hram[at - HRAM_S];

    return ie_reg;
#else
    return test_memory[at];
#endif
}


void Memory::write(u16 at, u8 data) {
#ifndef TEST
    if (at >= ROM_S && at <= ROM_E) 	    
        rom_banks[at - ROM_S] = data;
    
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

    else if (at >= IO_S && at <= IO_E)
        io_reg[at - IO_S] = (at != DIV) ? data : 0x00;

    else if (at >= HRAM_S && at <= HRAM_E) 	
        hram[at - HRAM_S] = data;

    else if (at == IE_REG) 	
        ie_reg = data;
#else
    test_memory[at] = data;
#endif
}

void Memory::reset() {
#define reset(arr) std::fill(arr.begin(), arr.end(), static_cast<u8>(0));

#ifndef TEST
    reset(rom_banks);
    reset(exram);
    reset(wram);
    reset(oam);
    reset(io_reg);
    reset(hram);

    ie_reg = 0;
    oam_lock = 0;
#else
    reset(test_memory);
#endif
}
