#include "ReminderManager.h"
#include "../app/Application.h"
#include "../rynth/RynthState.h"
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

	if (currentState == RynthState::IdleAware &&
		now - lastSwitch > (5000 + rand() % 5000)) {
		currentState = RynthState::Walking;
		lastSwitch = now;
	}
	
	static Uint32 lastScreenReminder = SDL_GetTicks();
	static Uint32 lastWaterReminder = SDL_GetTicks();

	// Reminder layar tiap 30 menit
	if (now - lastScreenReminder > 1800000) {
		bubble.show("Sudah 30 menit, yuk rehat sejenak.", 5000);
		lastScreenReminder = now;
	}

	// Reminder minum air tiap 15 menit
	if (now - lastWaterReminder > 900000) {
		bubble.show("Minum air putih ±150–200 ml.", 5000);
		lastWaterReminder = now;
	}
}