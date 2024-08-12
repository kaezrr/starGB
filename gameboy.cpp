#include "gameboy.hpp"
#include <iostream>
#include <format>

GameBoy::GameBoy(SDL_Renderer* renderer, SDL_Texture* texture)
	: ppu{ &memory, renderer, texture } { }

void GameBoy::no_boot_rom() {
	sm83.AF.full = 0x01B0;
	sm83.BC.full = 0x0013;
	sm83.DE.full = 0x00D8;
	sm83.HL.full = 0x014D;
	sm83.PC.full = 0x0100;
	sm83.SP.full = 0xFFFE;
	memory.write(LY, 0x90);
}

void GameBoy::run_instruction() {
	sm83.fetch_opcode();
	sm83.decode_opcode();
	sm83.handle_interrupts();
}

void GameBoy::start() {
	enabled = true;
	SDL_Event event{};

	if(!memory.execute_boot) no_boot_rom();

	while (enabled) { // Loop runs at 59.7 Hz
		sm83.elapsed_cycles = 0;
		auto start = std::chrono::steady_clock::now(); 
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				enabled = false;
				break;
			case SDL_KEYDOWN:
				set_button_on(event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				set_button_off(event.key.keysym.scancode);
				break;
			}
		}
		
		while (sm83.elapsed_cycles < 17556) {   // 17556 cycles per frame ~ 4.19MHz
#ifdef LOG
            debugger.write_match_log(sm83, memory);
#endif // LOG
			run_instruction();
        }

		auto delay = 16 - since(start).count(); 		
		if (delay <= 0) continue;
		SDL_Delay(static_cast<u32>(delay)); // Wait until 16ms has passed
	}
}

void GameBoy::load_game(const string& path) {
	std::ifstream program{ path, std::ios::binary };
	if (!program) {
		std::cerr << "File not found!\n";
		std::exit(1);
	}
	program.seekg(0, std::ios::end);
	size_t rom_size = program.tellg();
	program.seekg(0, std::ios::beg);
	if (rom_size > 0x8000) {
		std::cerr << "File too big!\n";
		std::exit(1);
	}
	program.read(reinterpret_cast<char*>(&memory.rom_banks[ROM_S]), rom_size);
}

void GameBoy::load_boot(const string& path) {
	std::ifstream program{ path, std::ios::binary };
	if (!program) {
		std::cerr << "File not found!\n";
		std::exit(1);
	}
	program.seekg(0, std::ios::end);
	size_t rom_size = program.tellg();
	program.seekg(0, std::ios::beg);
	if (rom_size > 0x100) {
		std::cerr << "File too big!\n";
		std::exit(1);
	}
	program.read(reinterpret_cast<char*>(&memory.boot_rom[0]), rom_size);
	memory.execute_boot = true;
}

void GameBoy::set_button_on(const SDL_Scancode& code) {
	switch (code) {
	case SDL_SCANCODE_UP:
		memory.input_buffer |= (1 << UP);
		break;
	case SDL_SCANCODE_DOWN:
		memory.input_buffer |= (1 << DOWN);
		break;
	case SDL_SCANCODE_LEFT:
		memory.input_buffer |= (1 << LEFT);
		break;
	case SDL_SCANCODE_RIGHT:
		memory.input_buffer |= (1 << RIGHT);
		break;
	case SDL_SCANCODE_S:
		memory.input_buffer |= (1 << A);
		break;
	case SDL_SCANCODE_A:
		memory.input_buffer |= (1 << B);
		break;
	case SDL_SCANCODE_RETURN:
		memory.input_buffer |= (1 << START);
		break;
	case SDL_SCANCODE_LSHIFT:
		memory.input_buffer |= (1 << SELECT);
		break;
	}
}

void GameBoy::set_button_off(const SDL_Scancode& code) {
	switch (code) {
	case SDL_SCANCODE_UP:
		memory.input_buffer &= ~(1 << UP);
		break;
	case SDL_SCANCODE_DOWN:
		memory.input_buffer &= ~(1 << DOWN);
		break;
	case SDL_SCANCODE_LEFT:
		memory.input_buffer &= ~(1 << LEFT);
		break;
	case SDL_SCANCODE_RIGHT:
		memory.input_buffer &= ~(1 << RIGHT);
		break;
	case SDL_SCANCODE_S:
		memory.input_buffer &= ~(1 << A);
		break;
	case SDL_SCANCODE_A:
		memory.input_buffer &= ~(1 << B);
		break;
	case SDL_SCANCODE_RETURN:
		memory.input_buffer &= ~(1 << START);
		break;
	case SDL_SCANCODE_LSHIFT:
		memory.input_buffer &= ~(1 << SELECT);
		break;
	}
}
