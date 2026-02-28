#include "Rynth.h"
#include "RynthState.h"
#include "../app/Application.h"
#include <SDL.h>

void Application::updateState(float deltaTime)
{
    // =========================
    // 1. UPDATE OVERLAY TIMER
    // =========================
    if (overlayTimer > 0.0f) {
        overlayTimer -= deltaTime / 1000.0f;
        if (overlayTimer <= 0.0f) {
            overlayState = RynthState::IdleAware;
			currentState = RynthState::Walking;
        }
    }

    // =========================
    // 2. IDLE TIMER (50 detik)
    // =========================
    idleTimer += deltaTime / 1000.0f;

	if (idleTimer >= 50.0f && overlayTimer <= 0.0f) {
		int r = rand() % 3;
		if (r == 0) currentState = RynthState::Sitting;
		else if (r == 1) currentState = RynthState::Sleeping;
		else currentState = RynthState::IdleAware;

		idleTimer = 0.0f;
	}

    // =========================
    // 3. PILIH SPRITE (OVERLAY PRIORITY)
    // =========================
    if (overlayState == RynthState::ClickedResponse) {
        testSprite = sprites["clicked"];
        return;
    }

    // =========================
    // 4. BASE STATE SPRITE
    // =========================
    switch (currentState) {
        case RynthState::Walking:
            testSprite = sprites["walking"];
            break;
        case RynthState::Sitting:
            testSprite = sprites["sitting"];
            break;
        case RynthState::Sleeping:
            testSprite = sprites["sleeping"];
            break;
        case RynthState::IdleBlink:
            testSprite = sprites["blink"];
            break;
        case RynthState::IdleSilent:
            testSprite = sprites["silent"];
            break;
        case RynthState::LightHappy:
            testSprite = sprites["happy"];
            break;
        case RynthState::IdleAware:
        default:
            testSprite = sprites["idle"];
            break;
    }
	
	// =========================
	// 5. WINDOW RESIZE ON STATE CHANGE
	// =========================
	if (appMode != AppMode::About && currentState != lastState) {
		updateWindowForState(currentState);
		lastState = currentState;
	}
}