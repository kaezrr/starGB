#include "apu.hpp"
#include "constants.hpp"
#include <SDL_audio.h>

void callback(void *user_data, u8 *stream, int len) {
    APU* apu = reinterpret_cast<APU*>(user_data);
    s16* sample_stream = reinterpret_cast<s16*>(stream);
    for(int i = 0; i < (len >> 1); ++i) {
        sample_stream[i] = (apu->ch1_get_sample() + apu->ch2_get_sample() +
                            apu->ch3_get_sample() + apu->ch4_get_sample()) / 4;
    }
}

void APU::pause(bool pause_on) { SDL_PauseAudioDevice(audio_device, pause_on); }

u8 APU::read(u16 at) const {
    switch (at) {
    case NR50:
        return nr50;
    case NR51:
        return nr51;
    case NR52:
        return nr52;
    // Audio Channel 1
    case NR10:
        return nr10;
    case NR11:
        return nr11;
    case NR12:
        return nr12;
    case NR13:
        return nr13;
    case NR14:
        return nr14;
    // Audio Channel 2
    case NR21:
        return nr21;
    case NR22:
        return nr22;
    case NR23:
        return nr23;
    case NR24:
        return nr24;
    // Audio Channel 3
    case NR30:
        return nr30;
    case NR31:
        return nr31;
    case NR32:
        return nr32;
    case NR33:
        return nr33;
    case NR34:
        return nr34;
    case WAVE_RAM_S ... WAVE_RAM_E:
        return wave_ram[at - WAVE_RAM_S];
    // Audio Channel 4
    case NR41:
        return nr41;
    case NR42:
        return nr42;
    case NR43:
        return nr43;
    case NR44:
        return nr44;
    }
    return 0xFF;
}

void APU::write(u16 at, u8 data) {
    switch (at) {
    case NR50:
        nr50 = data;
        return;
    case NR51:
        nr51 = data;
        return;
    case NR52:
        nr52 = data;
        return;
    // Audio Channel 1
    case NR10:
        nr10 = data;
        return;
    case NR11:
        nr11 = data;
        return;
    case NR12:
        nr12 = data;
        return;
    case NR13:
        nr13 = data;
        return;
    case NR14:
        nr14 = data;
        return;
    // Audio Channel 2
    case NR21:
        nr21 = data;
        return;
    case NR22:
        nr22 = data;
        return;
    case NR23:
        nr23 = data;
        return;
    case NR24:
        nr24 = data;
        return;
    // Audio Channel 3
    case NR30:
        nr30 = data;
        return;
    case NR31:
        nr31 = data;
        return;
    case NR32:
        nr32 = data;
        return;
    case NR33:
        nr33 = data;
        return;
    case NR34:
        nr34 = data;
        return;
    case WAVE_RAM_S ... WAVE_RAM_E:
        wave_ram[at - WAVE_RAM_S] = data;
        return;
    // Audio Channel 4
    case NR41:
        nr41 = data;
        return;
    case NR42:
        nr42 = data;
        return;
    case NR43:
        nr43 = data;
        return;
    case NR44:
        nr44 = data;
        return;
    }
}

s16 APU::ch1_get_sample() { return 0; }

s16 APU::ch2_get_sample() { return 0; }

s16 APU::ch3_get_sample() { return 0; }

s16 APU::ch4_get_sample() { return 0; }