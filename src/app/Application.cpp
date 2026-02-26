#include "Application.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cstdlib>
#include <ctime>

// Helper untuk load sprite
SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path, int& w, int& h) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) return nullptr;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    w = surface->w;
    h = surface->h;
    SDL_FreeSurface(surface);

    return tex;
}

Application::Application() {}
Application::~Application() {}

void Application::run()
{
    while (running)
    {
        float frameStart = SDL_GetTicks();

		float deltaTime = SDL_GetTicks() - frameStart;

        processEvents();		
		updateTimers(deltaTime);
		updateState(deltaTime);
		updateMovement(deltaTime);
        render();
        delayFrame(frameStart);
    }
}

void Application::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;

        if (event.type == SDL_MOUSEBUTTONDOWN) {
			currentState = RynthState::ClickedResponse;
			// pilih random kalimat klik
			int idx = rand() % 4;
			bubble.show(SpeechBubble::clickLines[idx], 3000);
		}
    }
}

void Application::delayFrame(float frameStart)
{
    float frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
        SDL_Delay(frameDelay - frameTime);
}