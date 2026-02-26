#pragma once
#include <map>
#include <string>
#include <SDL.h>
#include "RynthState.h"

class Rynth {
public:
    void updateState(RynthState state,
                     std::map<std::string, SDL_Texture*>& sprites,
                     SDL_Texture*& currentSprite);
};
