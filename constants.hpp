#pragma once
#include <cstdint>

// Type aliases
using u8    = uint8_t;
using s8    = int8_t;
using u16   = uint16_t;
using s16   = int16_t;
using u32   = uint32_t;
using s32   = int32_t;

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