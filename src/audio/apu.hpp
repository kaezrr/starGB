#pragma once
#include "constants.hpp"
#include <SDL_audio.h>
#include <array>

void callback(void* user_data, u8* stream, int len); 

struct APU {
    SDL_AudioSpec spec{
        .freq=65536,
        .format=AUDIO_S16,
        .channels=2,
        .samples=1024,
        .callback= callback,
        .userdata=this
    };

    SDL_AudioDeviceID audio_device{};

    u8 nr52{}, nr51{}, nr50{};
    u8 nr10{}, nr11{}, nr12{}, nr13{}, nr14{};
    u8 nr21{}, nr22{}, nr23{}, nr24{};
    u8 nr30{}, nr31{}, nr32{}, nr33{}, nr34{};
    u8 nr41{}, nr42{}, nr43{}, nr44{};
    std::array<u8, 16> wave_ram{};

    APU() {
        audio_device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
        pause(true);
    }

    ~APU() { SDL_CloseAudioDevice(audio_device); }

    u8 read(u16 at) const;
    void write(u16 at, u8 data);
    void pause(bool pause_on);

    s16 ch1_get_sample();
    s16 ch2_get_sample();
    s16 ch3_get_sample();
    s16 ch4_get_sample();
};