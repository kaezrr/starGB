#pragma once
#include <cstdint>
#include <cstddef>

// Type aliases
using u8    = uint8_t;
using s8    = int8_t;
using u16   = uint16_t;
using s16   = int16_t;
using u32   = uint32_t;
using s32   = int32_t;
using size_t = std::size_t;

constexpr auto PIXEL_SIZE       = 3;
constexpr auto SCREEN_WIDTH     = 160;
constexpr auto SCREEN_HEIGHT    = 144;

// Memory components addresses 
constexpr u16 ROM_S     = 0x0000;
constexpr u16 ROM_E     = 0x7FFF;
constexpr u16 VRAM_S    = 0x8000;
constexpr u16 VRAM_E    = 0x9FFF;
constexpr u16 EXRAM_S   = 0xA000;
constexpr u16 EXRAM_E   = 0xBFFF;
constexpr u16 WRAM_S    = 0xC000;
constexpr u16 WRAM_E    = 0xDFFF;
constexpr u16 ECHO_S    = 0xE000;
constexpr u16 ECHO_E    = 0xFDFF;
constexpr u16 OAM_S     = 0xFE00;
constexpr u16 OAM_E     = 0xFE9F;
constexpr u16 FORBID_S  = 0xFEA0;
constexpr u16 FORBID_E  = 0xFEFF;
constexpr u16 IO_S      = 0xFF00;
constexpr u16 IO_E      = 0xFF7F;
constexpr u16 HRAM_S    = 0xFF80;
constexpr u16 HRAM_E    = 0xFFFE;
constexpr u16 IE_REG    = 0xFFFF;

// Timer addresses
constexpr u16 DIV       = 0xFF04;
constexpr u16 TIMA      = 0xFF05;
constexpr u16 TMA       = 0xFF06;
constexpr u16 TAC       = 0xFF07;

// Interrupt register addresses
constexpr u16 IE        = 0xFFFF;
constexpr u16 IF        = 0xFF0F;

// IF bit aliases
constexpr u16 VBLANK    = 0x01;
constexpr u16 LCD       = 0x02;
constexpr u16 TIMER     = 0x04;
constexpr u16 SERIAL    = 0x08;
constexpr u16 JOYPAD    = 0x10;

// PPU registers
constexpr u16 LCDC      = 0xFF40;
constexpr u16 STAT      = 0xFF41;
constexpr u16 SCY       = 0xFF42;
constexpr u16 SCX       = 0xFF43;
constexpr u16 LY        = 0xFF44;
constexpr u16 LYC       = 0xFF45;
constexpr u16 DMA       = 0xFF46;
constexpr u16 BGP       = 0xFF47;
constexpr u16 OBP0      = 0xFF48;
constexpr u16 OBP1      = 0xFF49;
constexpr u16 WY        = 0xFF4A;
constexpr u16 WX        = 0xFF4B;

constexpr u16 TILE_MAP1 = 0x9800;
constexpr u16 TILE_MAP2 = 0x9C00;

// Serial Data Transfer registers
constexpr u16 SB = 0xFF01;
constexpr u16 SC = 0xFF02;

// Joypad
constexpr u16 JOYP = 0xFF00;

// Emulator color palette
constexpr u32 GB_DISABLED  = 0xBEE6B3;
constexpr u32 GB_PALETTE_0 = 0xA1EF8C;
constexpr u32 GB_PALETTE_1 = 0x3FAC95;
constexpr u32 GB_PALETTE_2 = 0x446176;
constexpr u32 GB_PALETTE_3 = 0x2C2137;
