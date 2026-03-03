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
        }
    }
	
	// =========================
	// alpha v0.2 — BLINK AUTO RETURN
	// =========================
	static float blinkDuration = 0.0f;

	if (currentState == RynthState::IdleBlink) {
		blinkDuration += deltaTime / 1000.0f;
		if (blinkDuration > 0.25f) { // blink cepat
			currentState = RynthState::IdleAware;
			blinkDuration = 0.0f;
		}
	}

    // =========================
    // 2. IDLE TIMER (50 detik)
    // =========================
    idleTimer += deltaTime / 1000.0f;

	if (idleTimer >= 50.0f && overlayTimer <= 0.0f &&
		currentState != RynthState::Walking) {
		int r = rand() % 3;
		if (r == 0) currentState = RynthState::Sitting;
		else if (r == 1) currentState = RynthState::Sleeping;
		else currentState = RynthState::IdleAware;

		idleTimer = 0.0f;
	}
	
	// =========================
	// alpha v0.2 — STATE TRANSITION (IDLE ↔ WALK)
	// =========================
	static float idleDecisionTimer = 0.0f;
	idleDecisionTimer += deltaTime / 1000.0f;

	// Setiap ~20–40 detik, mungkin jalan sebentar
	if (idleDecisionTimer > (20.0f + rand() % 20) &&
		overlayTimer <= 0.0f &&
		currentState == RynthState::IdleAware)
	{
		currentState = RynthState::Walking;
		idleDecisionTimer = 0.0f;
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