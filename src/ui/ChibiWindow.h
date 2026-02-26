#pragma once
#include <SDL.h>
#include "../ui/SpeechBubble.h"
#include "../rynth/RynthState.h"

class ChibiWindow {
public:
    bool initialize();
    void shutdown();
    void render(SDL_Texture* sprite, SpeechBubble& bubble,
                RynthState state, SDL_Rect& dst);

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};
