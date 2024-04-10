#pragma once

#include<cstddef>
#include<cstdint>
#include<vector>
#include<array>

using std::array;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;


class Memory {
private:
    array<u8, 32768> rom_banks{};
    array<u8, 8192>  vram{};
    array<u8, 8192>  exram{};
    array<u8, 8192>  wram{};
    array<u8, 160>   oam{};
    array<u8, 128>   io_reg{};
    array<u8, 127>   hram{};

    u8 ie_reg{};
    bool oam_lock{};

public:
    u8 read(u16 at) const;
    void write(u16 at, u8 data);

};