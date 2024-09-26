#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_version.h>
#include<spdlog/spdlog.h>

#include "version.h"
#include "gameboy.hpp"

int main(int argc, char** argv) {
    spdlog::set_pattern("%^[%l]%$ %v");
    if(argc < 2 || argc > 3) {
        spdlog::error("Usage: {} [path/to/rom] [path/to/boot-rom](optional)", argv[0]);
        return 1;
    }
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    GameBoy GB{};

    if(argc == 3) GB.load_boot(argv[2]);
    GB.load_game(argv[1]);

    spdlog::info("{} Version {}", PROJECT_NAME, PROJECT_VERSION);
    GB.start();

    spdlog::shutdown();
    SDL_Quit();
    return 0;
}