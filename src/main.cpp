#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>
#include <vector>
#include <SDL_version.h>
#include <spdlog/spdlog.h>

#include "version.h"
#include "gameboy.hpp"

void parse_args(int argc, char** argv, string& boot, string& log) {
    std::vector<string> args{argv, argv + argc};
    for (int i = 2; i < argc; i += 2) {
        if (args[i] == "-b")
            boot = args[i + 1];
        else if (args[i] == "-l")
            log = args[i + 1];
        else {
            spdlog::error("invalid flag");
            std::exit(1);
        };
    }
}

int main(int argc, char** argv) {
    spdlog::set_pattern("%^[%l]%$ %v");
    if(argc & 1 || argc > 6) {
        spdlog::error("Usage: {} [path/to/rom] -flags(optional)", argv[0]);
        spdlog::info("flags: -b [path/to/bootrom] -l [path/to/logs]");
        return 1;
    }

    //parsing arguments
    string boot_path{""}, log_path{""}, game_path{argv[1]};
    parse_args(argc, argv, boot_path, log_path);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    GameBoy GB{game_path, boot_path, log_path};
    spdlog::info("{} Version {}", PROJECT_NAME, PROJECT_VERSION);
    GB.start();

    spdlog::shutdown();
    SDL_Quit();
    return 0;
}