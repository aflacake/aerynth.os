#include "Application.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>

std::string Application::loadTextFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

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
    Uint32 lastTick = SDL_GetTicks();

    while (running)
	{
		Uint32 now = SDL_GetTicks();
		float deltaTime = static_cast<float>(now - lastTick);
		lastTick = now;

		processEvents();

		if (appMode == AppMode::Normal) {
			updateTimers(deltaTime);
			updateState(deltaTime);
			updateMovement(deltaTime);
		}

		render();
		delayFrame(now);
	}
}

void Application::processEvents()
{
    // =========================
    // ABOUT MODE EVENT LOOP
    // =========================
    if (appMode == AppMode::About) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT)
            {
                int mx = event.button.x;
                int my = event.button.y;

                SDL_Rect okBtn = { 250, 320, 100, 40 };

                if (mx >= okBtn.x && mx <= okBtn.x + okBtn.w &&
                    my >= okBtn.y && my <= okBtn.y + okBtn.h)
                {
                    appMode = AppMode::Normal;

                    SDL_SetWindowSize(window, 350, 600);
                    SDL_SetWindowPosition(
                        window,
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED
                    );
                }
            }
        }
        return; //STOP TOTAL
    }
	
    static bool dragging = false;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {	
		// =========================
		// ABOUT MODE: BLOCK GAME LOGIC
		// =========================
		if (appMode == AppMode::About) {
			if (event.type == SDL_QUIT)
				running = false;

			// klik OK ditangani khusus (kalau sudah kamu buat)
			continue;
		}
		
		// ABOUT SCROLL
		if (appMode == AppMode::About && event.type == SDL_MOUSEWHEEL) {
			aboutScrollY -= event.wheel.y * aboutScrollSpeed;
			if (aboutScrollY < 0) aboutScrollY = 0;
		}

        if (event.type == SDL_QUIT)
            running = false;

        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_ESCAPE)
            running = false;

        // =========================
        // KLIK KIRI (INTERAKSI)
        // =========================
        if (event.type == SDL_MOUSEBUTTONDOWN &&
            event.button.button == SDL_BUTTON_LEFT)
        {
            idleTimer = 0.0f; // ← SATU-SATUNYA TEMPAT RESET

            overlayState = RynthState::ClickedResponse;
            overlayTimer = 3.0f;

            int r = rand() % 4;
            bubble.show(SpeechBubble::clickLines[r], 3000);
        }

        // =========================
        // DRAG WINDOW (TIDAK RESET IDLE)
        // =========================
        if (event.type == SDL_MOUSEBUTTONDOWN &&
            event.button.button == SDL_BUTTON_MIDDLE)
        {
            dragging = true;
        }

        if (event.type == SDL_MOUSEBUTTONUP &&
            event.button.button == SDL_BUTTON_MIDDLE)
        {
            dragging = false;
        }

        if (event.type == SDL_MOUSEMOTION && dragging)
        {
            int winX, winY;
            SDL_GetWindowPosition(window, &winX, &winY);

            SDL_SetWindowPosition(
                window,
                winX + event.motion.xrel,
                winY + event.motion.yrel
            );
        }
    }
}

void Application::delayFrame(float frameStart)
{
    float frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
        SDL_Delay(frameDelay - frameTime);
}