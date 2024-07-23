#include "gameboy.hpp"

GameBoy::GameBoy(SDL_Renderer* renderer, SDL_Texture* texture)
	: ppu{ &memory, renderer, texture } { }

void GameBoy::run_instruction() {
	sm83.fetch_opcode();
	sm83.decode_opcode();

#ifdef LOG
	if(!sm83.halt_mode) debugger.write_match_log(sm83, memory);
#endif 

	sm83.handle_interrupts();
}

void GameBoy::start() {
	enabled = true;
	SDL_Event event{};
	no_boot_rom();

	while (enabled) { // Loop runs at 59.7 Hz
		sm83.elapsed_cycles = 0;
		auto start = std::chrono::steady_clock::now(); 

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				enabled = false;
				break;
			}
		}

		while (sm83.elapsed_cycles < 70224) { 
			run_instruction();
			if (memory.read(SC) == 0x81) {
				std::cout << memory.read(SB);
				memory.write(SC, 0);
			}
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

void GameBoy::no_boot_rom() {
	sm83.AF.full = 0x01B0;
	sm83.BC.full = 0x0013;
	sm83.DE.full = 0x00D8;
	sm83.HL.full = 0x014D;
	sm83.PC.full = 0x0100;
	sm83.SP.full = 0xFFFE;
	memory.write(LY, 0x90);
}
