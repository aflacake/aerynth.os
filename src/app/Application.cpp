#include "Application.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>

std::vector<std::string> Application::loadDialog(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '=') { // skip header
            lines.push_back(line);
        }
    }
    return lines;
}

// Helper untuk load sprite
SDL_Texture* Application::loadTexture(SDL_Renderer* renderer, const std::string& path, int& w, int& h) {
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
        if (event.type == SDL_QUIT) {
            running = false;
		}
		
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			running = false;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN &&
			event.button.button == SDL_BUTTON_RIGHT) {
			running = false;
		}

        if (event.type == SDL_MOUSEBUTTONDOWN) {
			idleTimer = 0.0f;
			overlayState = RynthState::ClickedResponse;
			overlayTimer = 3.0f;
			currentState = RynthState::ClickedResponse;
		}
    }
}

void Application::delayFrame(float frameStart)
{
    float frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
        SDL_Delay(frameDelay - frameTime);
}