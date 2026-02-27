#pragma once
#include <vector>
#include <string>
#include "../core/ReminderManager.h"
#include "../rynth/RynthMovement.h"
#include "../rynth/Rynth.h"
#include "../ui/ChibiWindow.h"
#include "../ui/SpeechBubble.h"

class Application {
public:
    Application();
    ~Application();

    bool initialize();
    void run();
    void updateTimers(float deltaTime);
    void updateState(float deltaTime);
    void updateMovement(float deltaTime);
    void render();
    void shutdown();

private:
    void processEvents();
    void delayFrame(float frameStartTime);
	SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path, int& w, int& h);

private:
	std::vector<std::string> loadDialog(const std::string& filename);

    bool running = false;
    const int targetFPS = 60;
    const float frameDelay = 1000.0f / targetFPS;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* testSprite = nullptr;
    int spriteW = 0;
    int spriteH = 0;
    SDL_Rect dst{0,0,0,0};

    RynthState currentState;
	RynthState overlayState;

	float overlayTimer = 0.0f;
	
	// idle tracking (fitur "dibiarkan lama")
	float idleTimer = 0.0f;
	
    std::map<std::string, SDL_Texture*> sprites;
    SpeechBubble bubble;
};
