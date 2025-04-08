#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cmath>
#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        SDL_Log("Mix_OpenAudio failed: %s", Mix_GetError());
        return -1;
    }

    Mix_Chunk* soundEffect = Mix_LoadWAV("test.wav");
    if (!soundEffect) {
        SDL_Log("Mix_LoadWAV failed: %s", Mix_GetError());
        return -1;
    }

    float listenerX = 0.0f;
    float listenerY = 0.0f;
    float listenerZ = 0.0f;

    float sourceX = 300.0f;
    float sourceY = -800.0f;
    float sourceZ = 0.0f;

    if (Mix_PlayChannel(-1, soundEffect, 0) == -1) {
        SDL_Log("Mix_PlayChannel failed: %s", Mix_GetError());
        return -1;
    }

    for (int i = 0; i < 500; i++) {
        float dx = sourceX - listenerX;
        float dy = sourceY - listenerY;
        float dz = sourceZ - listenerZ;

        float distance = sqrt(dx * dx + dy * dy + dz * dz);
        float angle = atan2(dy, dx);

        float pan = angle / M_PI;
        float maxDistance = 1000.0f;
        float volume = std::max(0.0f, 1.0f - (distance / maxDistance));

        Mix_SetPanning(0, (Uint8)((pan + 1.0f) * 127), (Uint8)((1.0f - pan) * 127));
        Mix_VolumeChunk(soundEffect, (int)(volume * MIX_MAX_VOLUME));

        sourceX = 300.0f * cos(i * 0.1f);
        sourceY = 300.0f * sin(i * 0.1f);
        sourceZ = 100.0f * cos(i * 0.1f);

        SDL_Delay(100);
    }

    Mix_FreeChunk(soundEffect);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
