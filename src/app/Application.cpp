#include "Application.h"
#include <SDL.h>

Application::Application() {}
Application::~Application() {}

bool Application::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        return false;

    window = SDL_CreateWindow(
		"Rynth",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		300, 300,
		SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP,
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	);

    if (!window)
        return false;

    renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED,
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	);

    if (!renderer)
        return false;

    running = true;
    return true;
	
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		return false;

	SDL_Surface* surface = IMG_Load(
		"assets/sprites/aware-and-idle-still-pose.png"
	);

	if (!surface)
		return false;

	testSprite = SDL_CreateTextureFromSurface(renderer, surface);
	spriteW = surface->w;
	spriteH = surface->h;
	SDL_FreeSurface(surface);

	if (!testSprite)
		return false;
}

void Application::run()
{
    while (running)
    {
        float frameStart = SDL_GetTicks();

        processEvents();
        updateTimers(0.0f);     // stub
        updateState(0.0f);      // stub
        updateMovement(0.0f);   // stub
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
}

void Application::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
    }
}

void Application::updateTimers(float) {}
void Application::updateState(float) {}
void Application::updateMovement(float) {}

void Application::render()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_Rect dst;
	dst.x = 0;
	dst.y = 0;
	dst.w = spriteW;
	dst.h = spriteH;

	SDL_RenderCopy(renderer, testSprite, nullptr, &dst);

	SDL_RenderPresent(renderer);
}

void Application::delayFrame(float frameStart)
{
    float frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
        SDL_Delay(frameDelay - frameTime);
}