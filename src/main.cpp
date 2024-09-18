#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include "gameboy.hpp"

int main(int argc, char*[] argv) {
    if(argc < 2 || argc > 3) {
        std::cout << "Usage: " << argv[0] << "[path/to/rom] " << "[path/to/boot-rom](optional)\n";
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    GameBoy GB{};

    if(arc == 3) 
        GB.load_boot(argv[2]);
    GB.load_game(argv[1]);

    GB.start();
    SDL_Quit();

    return 0;
}