#include "ReminderManager.h"
#include "../app/Application.h"
#include "../rynth/RynthState.h"
#include <ctime>
#include <SDL.h>
#include <cstdlib>

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
	
	static Uint32 lastScreenReminder = SDL_GetTicks();
	static Uint32 lastWaterReminder = SDL_GetTicks();
	
	// alpha v0.2 — speech cooldown
	static Uint32 lastSpeechTime = 0;
	const Uint32 speechCooldown = 10 * 60 * 1000; // 10 menit
	
	// Ambil waktu sekarang (jam)
	time_t t = time(nullptr);
	tm* nowTm = localtime(&t);
	int hour = nowTm->tm_hour;

	// Pilih dialog sesuai jam
	std::string line;
	if (hour >= 6 && hour < 10) {
		line = "Good morning… take it slow.";
	} else if (hour >= 10 && hour < 15) {
		line = "You've been at the screen for a while.";
	} else if (hour >= 15 && hour < 18) {
		line = "It's been a long day…";
	} else if (hour >= 18 && hour < 23) {
		line = "If you're tired, let's slow down.";
	}
	
	// Jangan bicara jika bubble masih aktif
	if (!bubble.canSpeak())
		return;

	// Jangan bicara jika baru saja bicara
	if (lastSpeechTime != 0 &&
		SDL_GetTicks() - lastSpeechTime < speechCooldown)
		return;

	// Reminder layar tiap 30 menit
	if (SDL_GetTicks() - lastScreenReminder > 1800000) {
		bubble.show(line, 5000);
		lastSpeechTime = SDL_GetTicks();
		lastScreenReminder = SDL_GetTicks();
	}

	// Reminder minum air tiap 15 menit
	if (SDL_GetTicks() - lastWaterReminder > 900000) {
		bubble.show("Drink water ±150–200 ml.", 5000);
		lastSpeechTime = SDL_GetTicks();
		lastWaterReminder = SDL_GetTicks();
	}
}