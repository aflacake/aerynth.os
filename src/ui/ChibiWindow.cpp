#include "ChibiWindow.h"
#include "../app/Application.h"
#include "../rynth/RynthState.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

bool Application::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        return false;

    // Set hint sebelum membuat window
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	window = SDL_CreateWindow(
		"Rynth",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		300, 600,
		SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
	);

	if (!window)
		return false;

	renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED
	);
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"); // untuk transparansi di Windows

	if (!renderer)
		return false;

	// Set blend mode setelah renderer berhasil dibuat
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    running = true;

	// init SDL_image
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		return false;
	
	if (TTF_Init() == -1) return false;

	// load sprite
	SDL_Surface* surface = IMG_Load("assets/sprites/aware-and-idle-still-pose.png");
	if (!surface)
		return false;

	testSprite = SDL_CreateTextureFromSurface(renderer, surface);
	spriteW = surface->w;
	spriteH = surface->h;
	
	dst.x = 0;
	dst.y = 0;
	dst.w = spriteW;
	dst.h = spriteH;

	SDL_FreeSurface(surface);

	if (!testSprite)
		return false;

	// Load semua sprite
	int w, h;
	sprites["idle"] = loadTexture(renderer, "assets/sprites/aware-and-idle-still-pose.png", w, h);
	sprites["clicked"] = loadTexture(renderer, "assets/sprites/clicked-pose.png", w, h);
	sprites["happy"] = loadTexture(renderer, "assets/sprites/light-happy-response-pose.png", w, h);
	sprites["walking"] = loadTexture(renderer, "assets/sprites/right-and-left-walking-pose.png", w, h);
	sprites["silent"] = loadTexture(renderer, "assets/sprites/silent-idle-pose.png", w, h);
	sprites["blink"] = loadTexture(renderer, "assets/sprites/silent-pose-aware-and-idle-blinking.png", w, h);
	sprites["sitting"] = loadTexture(renderer, "assets/sprites/sitting-pose.png", w, h);
	sprites["sleeping"] = loadTexture(renderer, "assets/sprites/sleeping-pose.png", w, h);

	// Set default sprite
	testSprite = sprites["idle"];
	spriteW = w;
	spriteH = h;

	return true; // pindahkan ke akhir setelah semua sukses
}

void Application::shutdown()
{
    if (testSprite) SDL_DestroyTexture(testSprite);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void Application::render()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	int winW, winH;
	SDL_GetWindowSize(window, &winW, &winH);

	this->dst.x = 0;
	this->dst.y = 0;
	this->dst.w = winW;
	this->dst.h = winH;
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // alpha 0 → transparan
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, testSprite, nullptr, &dst);
	
	// contoh sederhana: tampilkan bubble saat ClickedResponse
	if (currentState == RynthState::ClickedResponse) {
		// nanti diganti dengan class SpeechBubble
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
		SDL_Rect bubble = { 10, 10, 120, 40 };
		SDL_RenderFillRect(renderer, &bubble);
	}
	
	bubble.update();
	bubble.render(renderer, 20, 20);

	SDL_RenderPresent(renderer);
}