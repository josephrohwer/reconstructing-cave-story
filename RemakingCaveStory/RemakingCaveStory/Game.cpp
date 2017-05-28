#include "Game.h"
#include "SDL.h"
#include <stdio.h>
namespace {
	const int kScreenWidth = 640;
	const int kScreenHeight = 480;
	const int kFps = 60;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_ShowCursor(SDL_DISABLE);
	window_ = SDL_CreateWindow("Cave Story",
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, 
		kScreenWidth, kScreenHeight, 0);
	renderer_ = SDL_CreateRenderer(window_, -1, 0);
	eventLoop();
}

Game::~Game() {
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void Game::eventLoop() {
	// while (running). This runs 60 times per second.
	// Handle input.
	// update(). Move the player. Move projecticles. Check collisions.
	// draw(). Draw everything.
	SDL_Event event;
	bool running = true;
	while (running) {
		const int start_time_ms = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
				}
				break;
			default:
				break;
			}
		}

		update();
		draw();
		const int elapsed_time_ms = (SDL_GetTicks() - start_time_ms);
		// This loop lasts 1/60th of a second. So divide 1000 milliseconds by 60.
		// Need to subtract elapsed time because loop above also takes time.
		SDL_Delay(1000 /*ms*/ / kFps /*fps*/ - elapsed_time_ms /*ms*/);

		/* TESTS THE EVENT LOOP BY CHECKING IF IT IS RUNNING AT THE CORRECT FPS.
		const float seconds_per_frame = (SDL_GetTicks() - start_time_ms) / 1000.0;
		const float fps = 1 / seconds_per_frame;
		printf("fps=%f\n", fps);
		*/
	}
}

void Game::update() {

}

void Game::draw() {

}