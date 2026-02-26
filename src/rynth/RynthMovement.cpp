#include "RynthMovement.h"
#include "../app/Application.h"
#include "RynthState.h"
#include <SDL.h>
#include <cstdlib>
#include <cmath>

void Application::updateMovement(float deltaTime) {
    static int targetX = 0, targetY = 0;
    static bool hasTarget = false;

    if (currentState == RynthState::Walking) {
        if (!hasTarget) {
            int winW, winH;
            SDL_GetWindowSize(window, &winW, &winH);
            targetX = rand() % (winW - spriteW);
            targetY = rand() % (winH - spriteH);
            hasTarget = true;
        }

        // kecepatan piksel per detik
        float speed = 100.0f; 
        float step = speed * (deltaTime / 1000.0f);

        if (dst.x < targetX) dst.x += (int)step;
        if (dst.x > targetX) dst.x -= (int)step;
        if (dst.y < targetY) dst.y += (int)step;
        if (dst.y > targetY) dst.y -= (int)step;

        if (abs(dst.x - targetX) < 2 && abs(dst.y - targetY) < 2) {
            currentState = RynthState::IdleAware;
            hasTarget = false;
        }
    }
}