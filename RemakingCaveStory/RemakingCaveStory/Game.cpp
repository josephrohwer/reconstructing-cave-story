#include <stdio.h>
#include "Game.h"
#include "AnimatedSprite.h"

// An anonymous namespace. Can only be accessed within this file.
namespace {
	const int kFps = 60;
}

//STATIC
int Game::kTileSize = 32;

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING); //Initialize the SDL library. This must be called before using most other SDL functions.
	SDL_ShowCursor(SDL_DISABLE); //Don't show the cursor while the user is in the game.
	eventLoop();
}

Game::~Game() {
	SDL_Quit(); // Clean up all initialized subsystems. You should call it upon all exit conditions.
}

void Game::eventLoop() {
	Graphics graphics; // Creates the window and renderer.
	SDL_Event event; // A union that contains structures for the different event types.
	sprite_.reset(new AnimatedSprite(graphics, "MyChar", 0, 0, kTileSize, kTileSize, 15, 3));
	int last_update_time = SDL_GetTicks();
	bool running = true;
	// This loop runs however many times kFps is set to per second. In this case 60 times per second.
	while (running) {
		const int start_time_ms = SDL_GetTicks();
		// Handle input.
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
		const int current_time_ms = SDL_GetTicks();
		update(current_time_ms - last_update_time);
		last_update_time = current_time_ms;
		draw(graphics);
		//This is all to make sure the loop is running at the correct FPS.
		const int ms_per_frame = 1000/*ms*/ / kFps;
		// SDL_GetTicks - get the number of milliseconds since the SDL library initialization.
		const int elapsed_time_ms = SDL_GetTicks() - start_time_ms;
		if (elapsed_time_ms < ms_per_frame) {
			// Wait a specified number of milliseconds before returning.
			SDL_Delay(ms_per_frame - elapsed_time_ms);
		}
		/* TESTS THE EVENT LOOP BY CHECKING IF IT IS RUNNING AT THE CORRECT FPS.
		const float seconds_per_frame = (SDL_GetTicks() - start_time_ms) / 1000.0;
		const float fps = 1 / seconds_per_frame;
		printf("fps=%f\n", fps);
		*/
	}
}

// Move the player, move projecticles, and check collisions.
void Game::update(int elapsed_time_ms) {
	sprite_->update(elapsed_time_ms);
}

// Draw everything to the screen.
void Game::draw(Graphics& graphics) {
	graphics.clear();
	sprite_->draw(graphics, 320, 240);
	graphics.flip();
}
