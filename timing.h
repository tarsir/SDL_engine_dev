#ifndef _TIMING_H_
#define _TIMING_H_

#include "general.h"

clock_t timeStart, timeEnd, timeDiff; //for benchmarking and shiz

extern const int FPS;
int frame = 0;
bool cap = true;

class Timer {
	private:
		int startTicks, pausedTicks;
		bool paused, started;
	public:
		Timer() {
			startTicks = pausedTicks = 0;
			paused = started = false;
		}
		void start() {
			paused = false;
			started = true;
			startTicks = SDL_GetTicks();
		}

		void stop() {
			paused = true;
			started = false;
		}

		void pause() {
			if (started && !paused) {
				paused = true;
				pausedTicks = SDL_GetTicks() - startTicks;
			}
		}

		void unpause() {
			if (paused) {
				paused = false;
				startTicks = SDL_GetTicks() - pausedTicks;
				pausedTicks = 0;
			}
		}

		int getTicks() {
			if (started) {
				if (paused) {
					return pausedTicks;
				}
				else {
					return SDL_GetTicks() - startTicks;
				}
			}
			return 0;
		}

		bool isStarted() {
			return started;
		}
		bool isPaused() {
			return paused;
		}
};

Timer fps;

void fpsDelay() {
	if ((cap) && (fps.getTicks() < 1000/FPS)) {
		SDL_Delay((1000/FPS) - fps.getTicks());
	}
}

#endif
