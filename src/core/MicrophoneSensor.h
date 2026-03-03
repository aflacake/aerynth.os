#pragma once
#include <SDL.h>

class MicrophoneSensor {
public:
    bool init();
    void update();
    bool isSoundDetected() const;
    void shutdown();

private:
    SDL_AudioDeviceID device = 0;
    float volume = 0.0f;
	
	friend void audioCallback(void* userdata, Uint8* stream, int len);
};