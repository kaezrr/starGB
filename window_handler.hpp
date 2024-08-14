#pragma once
#include <SDL.h>
#include <array>
#include "constants.hpp"

using std::array;

struct Window_Handler {
    SDL_Window* window{};
    SDL_Texture* texture{};
    SDL_Renderer* renderer{};

    int s_height{}, s_width{}, pix_size{};
    array<u32, 4> colors{};

    Window_Handler(int height, int width, int pix,
        u32 color0, u32 color1, u32 color2, u32 color3); 
    ~Window_Handler();
    void render_frame(u8* display);
};

void handler_wrapper(void* ptr, u8* buffer);