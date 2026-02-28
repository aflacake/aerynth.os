#pragma once
#include <SDL.h>
#include <string>

class SpeechBubble {
public:
    SpeechBubble();
    ~SpeechBubble();

    void show(const std::string& text, int durationMs = 3000);
    void update();
    void render(SDL_Renderer* renderer, int x, int y);

    bool isActive() const;
	
	static const char* clickLines[4];
	
	void showPersistent(const std::string& text);
    void hide();
    bool isVisible() const;
	
	bool hitClose(int mouseX, int mouseY) const;

private:
	std::string text;
    std::string message;
    Uint32 startTime;
    int duration;
    bool active;
	
	bool persistent = false;
	
	SDL_Rect bubbleRect;
};
