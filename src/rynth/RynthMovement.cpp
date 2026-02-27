#include "RynthMovement.h"
#include "../app/Application.h"
#include "RynthState.h"
#include <SDL.h>
#include <cstdlib>
#include <cmath>

void Application::updateMovement(float deltaTime)
{
    static int targetX = 0;
    static bool hasTarget = false;

    // Jangan bergerak hanya kalau tidur
    if (currentState == RynthState::Sleeping)
        return;
	
	if (currentState != RynthState::Sleeping)
		currentState = RynthState::Walking;

    // Pastikan selalu kembali ke Walking
    if (currentState != RynthState::Walking)
        currentState = RynthState::Walking;

    if (!hasTarget) {
        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);
        targetX = rand() % (winW - spriteW);
        hasTarget = true;
    }

    float speed = 80.0f;
    float step = speed * (deltaTime / 1000.0f);

    if (dst.x < targetX) dst.x += (int)step;
    if (dst.x > targetX) dst.x -= (int)step;

    if (abs(dst.x - targetX) < 2) {
        hasTarget = false;
    }
}