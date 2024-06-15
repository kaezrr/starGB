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

enum class Fetcher {
    state1,
    state2,
};

struct PPU {
private:
    Memory* memory{};
    u32 queue_sp{};
    u16 queue_bg{};

    int bg_count{ 0 }, dots{ 0 }, x_pos{ 0 };
    int curr_sprite_location{ OAM_S };
    int window_line_counter{};

    u16 bg_data{};
    u32 sp_data{};
    u8 bg_tile_no{};
    u8 sp_tile_no{};

    Mode mode{ Mode::OAM_SCAN };
    vector<Sprite> sprite_buffer;
    vector<u16> colors{ GB_PALETTE_0, GB_PALETTE_1, GB_PALETTE_2, GB_PALETTE_3 };
    vector<vector<u32>> display = vector<vector<u32>>(144, vector<u32>(160));

    bool wy_cond{}, wx_cond{};

public:
    PPU(Memory* mem_ptr);
    
    void tick();
    void add_sprite(u16 at);
    bool display_window();
    void update_lyc();
    void update_mode();
    void increment_ly();

    void oam_scan();
    void vblank();
    void hblank();
    void drawing();

    void bg_fetch_tile_no();
    void bg_fetch_tile_data(bool state);
    void bg_push_to_fifo();
    bool sp_fetch_tile_no();
    void sp_fetch_tile_data(bool state);
    void sp_push_to_fifo();
    void push_to_display();
};