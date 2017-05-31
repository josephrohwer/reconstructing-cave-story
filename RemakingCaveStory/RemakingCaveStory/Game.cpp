#include <stdio.h>
#include "Game.h"
#include "Graphics.h"
#include "Player.h"
#include "Input.h"
#include "AnimatedSprite.h"

// An anonymous namespace. Can only be accessed within this file.
namespace {
	const int kFps = 60;
}

//STATIC : ONLY ONE COPY OF THIS IS EVER MADE, NO MATTER HOW MANY OBJECTS ARE CREATED.
int Game::kTileSize = 32;

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING); //Initialize the SDL library. This must be called before using most other SDL functions.
	SDL_ShowCursor(SDL_DISABLE); //Don't show the cursor while the user is in the game.
	eventLoop();
}

Game::~Game() {
	SDL_Quit(); // Clean up all initialized subsystems. Call it upon all exit conditions.
}

void Game::eventLoop() {
	Graphics graphics; // Creates the window and renderer.
	Input input; // Key events.
	SDL_Event event; // A union that contains structures for the different event types.
	player_.reset(new Player(graphics, 320, 240));
	int last_update_time = SDL_GetTicks();
	input.beginNewFrame(); // Clears pressed and released keys from the map.
	bool running = true;
	// This loop runs however many times kFps is set to per second. In this case 60 times per second.
	while (running) {
		const int start_time_ms = SDL_GetTicks();
		// Handle input.
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				input.keyDownEvent(event);
				break;
			case SDL_KEYUP:
				input.keyUpEvent(event);
				break;
			default:
				break;
			}
		}

		if (input.wasKeyPressed(SDLK_ESCAPE)) {
			running = false;
		}

		if (input.isKeyHeld(SDLK_LEFT) && input.isKeyHeld(SDLK_RIGHT)) {
			player_->stopMoving();
		}
		else if (input.isKeyHeld(SDLK_LEFT)) {
			player_->startMovingLeft();
		}
		else if (input.isKeyHeld(SDLK_RIGHT)) {
			player_->startMovingRight();
		}
		else {
			player_->stopMoving();
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
	player_->update(elapsed_time_ms);
}

// Draw everything to the screen.
void Game::draw(Graphics& graphics) {
	graphics.clear();
	player_->draw(graphics);
	graphics.flip();
}
