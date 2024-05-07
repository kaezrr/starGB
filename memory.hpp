#pragma once

#include<cstddef>
#include<cstdint>
#include<algorithm>
#include<vector>

using std::vector;
using u8  = uint8_t;
using s8  = int8_t;
using u16 = uint16_t;
using s16 = int16_t;
using u32 = uint32_t;
using s32 = int32_t;

//#define TEST

constexpr auto DIV  = 0xFF04;
constexpr auto TIMA = 0xFF05;
constexpr auto TMA  = 0xFF06;
constexpr auto TAC  = 0xFF07;

class Memory {
private:
#ifndef TEST
    vector<u8> rom_banks    = vector<u8>(32768);
    vector<u8> vram         = vector<u8>(8192);
    vector<u8> exram        = vector<u8>(8192);
    vector<u8> wram         = vector<u8>(8192);
    vector<u8> oam          = vector<u8>(160);
    vector<u8> io_reg       = vector<u8>(128);
    vector<u8> hram         = vector<u8>(127);

    u8 ie_reg{};
    bool oam_lock{};
#else 
    vector<u8> test_memory  = vector<u8>(65536);
#endif

public:
    u8 read(u16 at) const;
    void write(u16 at, u8 data);
    void reset();
};