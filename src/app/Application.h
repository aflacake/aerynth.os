#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>
#include "../ui/SpeechBubble.h"

class Application
{
enum class RynthState {
    IdleAware,
    IdleBlink,
    Walking,
    Sitting,
    Sleeping,
    ClickedResponse,
    LightHappy,
    IdleSilent
};

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
	
	SDL_Rect dst = {0, 0, 0, 0};
	
	std::map<std::string, SDL_Texture*> sprites;
	
	RynthState currentState = RynthState::IdleAware;
	
	SpeechBubble bubble;
};