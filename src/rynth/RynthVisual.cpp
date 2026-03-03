#include "RynthVisual.h"
#include <cstdlib>

// alpha v0.2 — idle blink logic
void RynthVisual::update(RynthState& state, float deltaTime)
{
    static float blinkTimer = 0.0f;
    static float nextBlinkTime = 0.0f;

    // Hanya boleh blink saat idle aware
    if (state != RynthState::IdleAware) {
        blinkTimer = 0.0f;
        return;
    }

    blinkTimer += deltaTime / 1000.0f;

    // Tentukan waktu blink berikutnya
    if (nextBlinkTime <= 0.0f) {
        // blink jarang: 5–12 detik
        nextBlinkTime = 5.0f + (rand() % 8);
    }

    // Saat waktunya blink
    if (blinkTimer >= nextBlinkTime) {
        state = RynthState::IdleBlink;
        blinkTimer = 0.0f;
        nextBlinkTime = 0.0f;
    }
}