#pragma once
#include <SDL.h>
#include "RynthState.h"

class RynthMovement {
public:
    void update(RynthState& state, SDL_Rect& dst, int spriteW, int spriteH,
                SDL_Window* window, float deltaTime);
};
