#pragma warning(push)
#pragma warning(disable : 26819)
#include <SDL.h>
#pragma warning(pop)

#include "gameboy.hpp"

int main(int, char*[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window* window = SDL_CreateWindow("StarGB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        PIXEL_SIZE * SCREEN_WIDTH, PIXEL_SIZE * SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
        SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    //SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);

    static GameBoy GB{ renderer, texture };

    //GB.load_boot("roms/dmg_boot.bin");
    GB.load_game("roms/dmg-acid2.gb");
    //GB.load_game("roms/tetris.gb");
    GB.start();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}