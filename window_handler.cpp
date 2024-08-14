#include "window_handler.hpp"


Window_Handler::Window_Handler(int height, int width, int pix,
    u32 color0, u32 color1, u32 color2, u32 color3) {
    s_height = height; s_width = width; pix_size = pix;
    window = SDL_CreateWindow("StarGB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        pix_size * s_width, pix_size * s_height, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
        SDL_TEXTUREACCESS_STREAMING, s_width, s_height);
    SDL_RaiseWindow(window);
	colors[0] = color0; colors[1] = color1;
	colors[2] = color2; colors[3] = color3;
}

Window_Handler::~Window_Handler() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Window_Handler::render_frame(u8* display) {
    int pitch = s_width * 4;
    int screen_size = s_width * s_height;
    u32* pixels{ nullptr };
    SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch);
    for (int i = 0; i < screen_size; ++i) 
        *pixels++ = colors[display[i]];
    pixels -= screen_size;
    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

void handler_wrapper(void* ptr, u8* buffer) {
    return static_cast<Window_Handler*>(ptr)->render_frame(buffer);
}
