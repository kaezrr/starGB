#include <SDL_video.h>

#include "gameboy.hpp"
#include "constants.hpp"
#include "debug.hpp"

GameBoy::GameBoy(const string &game, const string &boot, const string &log) {
	memory.load_game(game);
	memory.load_boot(boot);
	if(log != "") debugger.set_log_path(log);
}

void GameBoy::run_instruction() {
	sm83.fetch_opcode();
	sm83.decode_opcode();
	sm83.handle_interrupts();
}

void GameBoy::start() {
	handler.init(
        "StarGB",
        SCREEN_HEIGHT, SCREEN_WIDTH, 3,
        0xA1EF8C, 0x3FAC95,
        0x446176, 0x2C2137
	);
    debugger.tiles.init(
    	"VRAM Viewer",
    	TILE_HEIGHT * TILE_WINDOW_HEIGHT,
    	TILE_WIDTH * TILE_WINDOW_WIDTH, 2,
    	0xA1EF8C, 0x3FAC95,
    	0x446176, 0x2C2137,
		436, 324
    );

	enabled = true;
	SDL_RaiseWindow(handler.window);

	int frames_elapsed = 0;
	while (enabled) { // Loop runs at 59.7 Hz
		auto start = std::chrono::steady_clock::now(); 
		handle_events();
		debugger.render_tiles();

		while (sm83.elapsed_cycles < 17556) {   // 17556 cycles per frame ~ 4.19MHz
			if(debugger.enabled) debugger.write_text_log();
			run_instruction();
			sm83.cycle_parts(sm83.m_cycles);
        }

		frames_elapsed++;
		if(frames_elapsed >= 60) { // Save the game every 60 frames ~ 1 second
			memory.mbc->save_ram();
			frames_elapsed = 0;
		}

		sm83.elapsed_cycles = 0;
		auto delay = 16 - since(start).count(); 		
		if (delay <= 0) continue;
		SDL_Delay(static_cast<u32>(delay)); // Wait until 16ms has passed
	}
}

void GameBoy::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            enabled = false;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                enabled = false;
                break;
            }
            break;
        case SDL_KEYDOWN:
            set_button_on(event.key.keysym.scancode);
            break;
        case SDL_KEYUP:
            set_button_off(event.key.keysym.scancode);
            break;
        }
    }
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
	default: break;
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
	default: break;
	}
}