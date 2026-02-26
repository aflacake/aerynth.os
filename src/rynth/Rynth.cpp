#include "Rynth.h"
#include "RynthState.h"
#include "../app/Application.h"
#include <SDL.h>

void Application::updateState(float) {
    // Contoh sederhana: default IdleAware
    switch (currentState) {
        case RynthState::IdleAware:
            testSprite = sprites["idle"];
            break;
        case RynthState::IdleBlink:
            testSprite = sprites["blink"];
            break;
        case RynthState::Walking:
            testSprite = sprites["walking"];
            break;
        case RynthState::Sitting:
            testSprite = sprites["sitting"];
            break;
        case RynthState::Sleeping:
            testSprite = sprites["sleeping"];
            break;
        case RynthState::ClickedResponse:
            testSprite = sprites["clicked"];
            break;
        case RynthState::LightHappy:
            testSprite = sprites["happy"];
            break;
        case RynthState::IdleSilent:
            testSprite = sprites["silent"];
            break;
    }
}