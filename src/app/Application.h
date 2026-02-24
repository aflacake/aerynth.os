#pragma once
#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>

class Application
{
public:
    Application();
    ~Application();

    bool initialize();
    void run();
    void shutdown();

private:
    void processEvents();
    void updateTimers(float deltaTime);
    void updateState(float deltaTime);
    void updateMovement(float deltaTime);
    void render();
    void delayFrame(float frameStartTime);

private:
    bool running = false;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    const int targetFPS = 60;
    const float frameDelay = 1000.0f / targetFPS;
	
	SDL_Texture* testSprite = nullptr;
	int spriteW = 0;
	int spriteH = 0;
};