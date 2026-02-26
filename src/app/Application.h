#pragma once
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

private:
    bool running = false;
    const int targetFPS = 60;
    const float frameDelay = 1000.0f / targetFPS;

    SDL_Texture* testSprite = nullptr;
	int spriteW = 0;
	int spriteH = 0;
	SDL_Rect dst{0,0,0,0};
	bool running = false;

    ReminderManager reminderManager;
    RynthMovement rynthMovement;
    Rynth rynth;
    ChibiWindow chibiWindow;
    SDL_Window* window;
	SDL_Renderer* renderer;
	int spriteW, spriteH;
	SDL_Rect dst;
	bool running;
	RynthState currentState;
	std::map<std::string, SDL_Texture*> sprites;
	SpeechBubble bubble;

	SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path, int& w, int& h);
};
