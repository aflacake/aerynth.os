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
    static float walkTimer = 0.0f;

    // Hanya bergerak saat state WALKING
    if (currentState != RynthState::Walking)
        return;

    walkTimer += deltaTime / 1000.0f;

    // Tentukan target sekali saja
    if (!hasTarget) {
        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);

        // Jarak pendek (30–120 px)
        int offset = 30 + rand() % 90;
        targetX = dst.x + ((rand() % 2 == 0) ? -offset : offset);

        // Clamp agar tidak keluar window
        if (targetX < 0) targetX = 0;
        if (targetX > winW - dst.w)
            targetX = winW - dst.w;

        hasTarget = true;
        walkTimer = 0.0f;
    }

    float speed = 60.0f; // pelan
    float step = speed * (deltaTime / 1000.0f);

    if (dst.x < targetX) dst.x += (int)step;
    if (dst.x > targetX) dst.x -= (int)step;

    // Selesai jalan
    if (abs(dst.x - targetX) < 2 || walkTimer > 3.0f) {
        hasTarget = false;
        walkTimer = 0.0f;
        currentState = RynthState::IdleAware; // KEMBALI DIAM
    }
}