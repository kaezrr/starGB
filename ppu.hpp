#include "constants.hpp"
#include "memory.hpp"

struct Sprite {
    u8 posY{};
    u8 posX{};
    u8 tile_id{};

    bool obj_priority{};
    bool flipY{};
    bool flipX{};
    bool palette{};

    Sprite(u16 at, Memory& mem);
};

enum class Mode {
    HBLANK,
    VBLANK,
    OAM_SCAN,
    DRAWING,
};

struct PPU {
private:
    Memory* memory{};
    u32 queue_sp{};
    u16 queue_bg{};

    int sp_count{}, bg_count{};
    int dots{ 0 }, x_pos{ 0 };
    int curr_sprite_location{OAM_S};
    int window_line_counter{};

    Mode mode{ Mode::OAM_SCAN };
    vector<Sprite> sprite_buffer;

public:
    PPU(Memory* mem_ptr);

    void tick();

    void oam_scan();
    void add_sprite(u16 at);

    void vblank();
    void hblank();

    void fetch_tile_no();
    void fetch_tile_data(bool state);
    void push_to_fifo();
};