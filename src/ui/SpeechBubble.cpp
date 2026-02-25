#include "SpeechBubble.h"
#include <SDL.h>
#include <SDL_ttf.h>

SpeechBubble::SpeechBubble() : startTime(0), duration(0), active(false) {}
SpeechBubble::~SpeechBubble() {}

const char* SpeechBubble::clickLines[4] = {
    "Hmm?",
    "Yes?",
    "I hear you.",
    "What's up?"
};

void SpeechBubble::show(const std::string& text, int durationMs) {
    message = text;
    startTime = SDL_GetTicks();
    duration = durationMs;
    active = true;
}

void SpeechBubble::update() {
    if (active && SDL_GetTicks() - startTime > duration) {
        active = false;
    }
}

#include <SDL_ttf.h>   // pastikan ada di atas file

void SpeechBubble::render(SDL_Renderer* renderer, int x, int y) {
    if (!active) return;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_Rect bubble = { x, y, 200, 50 };
    SDL_RenderFillRect(renderer, &bubble);

    // Render teks dengan font Arial
    TTF_Font* font = TTF_OpenFont("assets/ui/arial.ttf", 16);
    if (font) {
        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, message.c_str(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = { x + 10, y + 10, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }
        TTF_CloseFont(font);
    }
}

bool SpeechBubble::isActive() const {
    return active;
}
