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

void Application::shutdown()
{
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
	if (testSprite) SDL_DestroyTexture(testSprite);
	IMG_Quit();
	TTF_Quit();
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

void Application::updateTimers(float) {
	static Uint32 lastClick = 0;
    if (currentState == RynthState::ClickedResponse) {
        if (lastClick == 0) lastClick = SDL_GetTicks();
        if (SDL_GetTicks() - lastClick > 2000) { // 2 detik
            currentState = RynthState::IdleAware;
            lastClick = 0;
        }
    }
	
    static Uint32 lastSwitch = SDL_GetTicks();
	Uint32 now = SDL_GetTicks();

	if (currentState == RynthState::IdleAware &&
		now - lastSwitch > (5000 + rand() % 5000)) {
		currentState = RynthState::Walking;
		lastSwitch = now;
	}
}

void Application::updateState(float) {
    // Contoh sederhana: default IdleAware
    switch (currentState) {
        case RynthState::IdleAware:
            testSprite = sprites["idle"];
            break;
        case RynthState::IdleBlink:
            testSprite = sprites["blink"];
            break;
        case RynthState::Walking:
            testSprite = sprites["walking"];
            break;
        case RynthState::Sitting:
            testSprite = sprites["sitting"];
            break;
        case RynthState::Sleeping:
            testSprite = sprites["sleeping"];
            break;
        case RynthState::ClickedResponse:
            testSprite = sprites["clicked"];
            break;
        case RynthState::LightHappy:
            testSprite = sprites["happy"];
            break;
        case RynthState::IdleSilent:
            testSprite = sprites["silent"];
            break;
    }
}

void Application::updateMovement(float deltaTime) {
    static int targetX = 0, targetY = 0;
    static bool hasTarget = false;

    if (currentState == RynthState::Walking) {
        if (!hasTarget) {
            int winW, winH;
            SDL_GetWindowSize(window, &winW, &winH);
            targetX = rand() % (winW - spriteW);
            targetY = rand() % (winH - spriteH);
            hasTarget = true;
        }

        if (dst.x < targetX) dst.x++;
        if (dst.x > targetX) dst.x--;
        if (dst.y < targetY) dst.y++;
        if (dst.y > targetY) dst.y--;

        if (dst.x == targetX && dst.y == targetY) {
            currentState = RynthState::IdleAware;
            hasTarget = false;
        }
    }
}

void Application::render()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_Rect dst;
	int winW, winH;
	SDL_GetWindowSize(window, &winW, &winH);

	dst.x = 0;
	dst.y = 0;
	dst.w = winW;
	dst.h = winH;
	
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

void Application::delayFrame(float frameStart)
{
    float frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
        SDL_Delay(frameDelay - frameTime);
}