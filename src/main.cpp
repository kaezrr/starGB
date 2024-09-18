#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "gameboy.hpp"

int main(int, char*[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    GameBoy GB{};

    //GB.load_boot("roms/dmg_boot.bin");
    GB.load_game("roms/tennis.gb");

    GB.start();
    SDL_Quit();

    return 0;
}