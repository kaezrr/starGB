#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "window_handler.hpp"
#include "sm83.hpp"
#include "memory.hpp"

using std::vector, std::string, std::tuple;

constexpr auto TILE_WIDTH = 8;
constexpr auto TILE_HEIGHT = 8;
constexpr auto TILE_SIZE = 64;
constexpr auto TILE_WINDOW_WIDTH = 16;
constexpr auto TILE_WINDOW_HEIGHT = 24;

struct Debugger {
    std::shared_ptr<spdlog::logger> logger = spdlog::rotating_logger_mt<spdlog::async_factory>(
        "logger",
        "logfile.txt", 
        1048576 * 5,
        3
    );
    const Memory* mem{ nullptr };
    const CPU* sm83{ nullptr };

    vector<u8> tile_buffer = vector<u8>(0x6000);
    Window_Handler tiles{};

    Debugger(Memory* mptr, CPU* cptr) : mem{ mptr }, sm83{ cptr } {
        spdlog::flush_every(std::chrono::seconds(3));
    }

    void log_path(const string& path);
    void write_text_log();
    void write_match_log();
    void print_cpu_mem(std::ostream& out);
    void memory_dump(u16 start, u16 end);

    void render_tiles();
    void fill_tile_data(u16 index);
};