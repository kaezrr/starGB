#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_version.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

#include "gameboy.hpp"
#include "version.h"

void parse_args(int argc, char** argv, string& log) {
    std::vector<string> args{argv, argv + argc};
    for (int i = 2; i < argc; i += 2) {
        if (args[i] == "-l")
            log = args[i + 1];
        else {
            spdlog::error("invalid flag");
            std::exit(1);
        };
    }
}

int main(int argc, char** argv) {
    spdlog::set_pattern("%^[%l]%$ %v");
    if (argc & 1 || argc > 4) {
        spdlog::error("Usage: {} [path/to/rom] -flags(optional)", argv[0]);
        spdlog::info("flags: -l [path/to/logs]");
        return 1;
    }

    // parsing arguments
    string log_path{""}, game_path{argv[1]};
    parse_args(argc, argv, log_path);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    GameBoy GB{game_path, log_path};
    spdlog::info("{} Version {}", PROJECT_NAME, PROJECT_VERSION);
    GB.start();

    spdlog::shutdown();
    SDL_Quit();
    return 0;
}
