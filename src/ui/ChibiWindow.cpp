#include "ChibiWindow.h"
#include "../app/Application.h"
#include "../rynth/RynthState.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>
#include <sstream>

bool Application::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        return false;

    // Set hint sebelum membuat window
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	float scale = 0.5f;
	window = SDL_CreateWindow(
		"Rynth",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		350, 600,
		SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
	);

	if (!window)
		return false;

	renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED
	);
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if (!renderer)
		return false;

	// Set blend mode setelah renderer berhasil dibuat
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    running = true;

	// init SDL_image
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		return false;
	
	if (TTF_Init() == -1) return false;
	
	// =========================
	// FIRST RUN – ABOUT MODE
	// =========================
	aboutText = loadTextFile("assets/about/ABOUT.md");
	appMode = AppMode::About;
	bool skipSpriteSizing = (appMode == AppMode::About);
	SDL_SetWindowSize(window, 600, 400);
	
	SDL_SetWindowPosition(window,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED);

	// load sprite
	SDL_Surface* surface = IMG_Load("assets/sprites/aware-and-idle-still-pose.png");
	if (!surface)
		return false;

	testSprite = SDL_CreateTextureFromSurface(renderer, surface);
	spriteW = surface->w;
	spriteH = surface->h;
	
	if (!skipSpriteSizing) {
		int winW = static_cast<int>(spriteW * scale);
		int winH = static_cast<int>(spriteH * scale);

		SDL_SetWindowSize(window, winW, winH);
	}
	
	if (!skipSpriteSizing) {
		dst.x = 0;
		dst.y = 0;
		dst.w = static_cast<int>(spriteW * scale);
		dst.h = static_cast<int>(spriteH * scale);
	}

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
	
	currentState = RynthState::Walking;      // baseline: selalu hidup
	overlayState = RynthState::IdleAware;    // tidak ada overlay
	overlayTimer = 0.0f;
	idleTimer = 0.0f;

	return true; // pindahkan ke akhir setelah semua sukses
}

void Application::updateWindowForState(RynthState state)
{
    if (!window) return;
    if (appMode == AppMode::About) return; // IMMUNE

    if (state == RynthState::Sleeping) {
        SDL_SetWindowSize(window, 600, 350);
    } else {
        SDL_SetWindowSize(window, 350, 600);
    }
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
	if (appMode == AppMode::About) {
		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderClear(renderer);

		// BACKGROUND PANEL
		SDL_Rect panel = { 20, 20, 560, 360 };
		SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
		SDL_RenderFillRect(renderer, &panel);

		// TEXT
		TTF_Font* font = TTF_OpenFont("assets/ui/arial.ttf", 16);
		if (font) {
			SDL_Color color = {0, 0, 0, 255};
			SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(
				font,
				aboutText.c_str(),
				color,
				520
			);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

			SDL_Rect textRect = {
				40,
				40 - aboutScrollY,
				surf->w,
				surf->h
			};
			SDL_RenderCopy(renderer, tex, nullptr, &textRect);

			SDL_FreeSurface(surf);
			SDL_DestroyTexture(tex);
			TTF_CloseFont(font);
		}

		// OK BUTTON
		SDL_Rect okBtn = { 250, 320, 100, 40 };
		SDL_SetRenderDrawColor(renderer, 60, 120, 220, 255);
		SDL_RenderFillRect(renderer, &okBtn);
		
		// OK BUTTON TEXT
		TTF_Font* btnFont = TTF_OpenFont("assets/ui/arial.ttf", 18);
		if (btnFont) {
			SDL_Color white = {255, 255, 255, 255};
			SDL_Surface* txtSurf = TTF_RenderText_Blended(btnFont, "OK", white);
			if (txtSurf) {
				SDL_Texture* txtTex = SDL_CreateTextureFromSurface(renderer, txtSurf);

				SDL_Rect txtRect;
				txtRect.w = txtSurf->w;
				txtRect.h = txtSurf->h;
				txtRect.x = okBtn.x + (okBtn.w - txtRect.w) / 2;
				txtRect.y = okBtn.y + (okBtn.h - txtRect.h) / 2;

				SDL_RenderCopy(renderer, txtTex, nullptr, &txtRect);

				SDL_DestroyTexture(txtTex);
				SDL_FreeSurface(txtSurf);
			}
			TTF_CloseFont(btnFont);
		}

		SDL_RenderPresent(renderer);
		return; // STOP render normal
	}
	
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    SDL_Texture* activeSprite = testSprite;
	
	int texW = 0, texH = 0;
	SDL_QueryTexture(activeSprite, nullptr, nullptr, &texW, &texH);

    float scaleX = (float)winW / texW;
	float scaleY = (float)winH / texH;
	float scale = (scaleX < scaleY) ? scaleX : scaleY;

	dst.w = static_cast<int>(texW * scale);
	dst.h = static_cast<int>(texH * scale);
	dst.x = (winW - dst.w) / 2;
	dst.y = (winH - dst.h) / 2;

	SDL_RenderCopy(renderer, testSprite, nullptr, &dst);

    bubble.update();
    bubble.render(renderer, 20, 20);

    SDL_RenderPresent(renderer);
}