#include <stdio.h>
#include "Game.h"
#include "Graphics.h"
#include "Player.h"
#include "Input.h"
#include "AnimatedSprite.h"
#include "Map.h"

// An anonymous namespace. Can only be accessed within this file.
namespace {
	const units::FPS kFps = 60;
}

// STATIC : ONLY ONE COPY OF THIS IS EVER MADE, NO MATTER HOW MANY OBJECTS ARE CREATED.
int Game::kTileSize = 32;
int Game::kScreenWidth = 20 * Game::kTileSize;
int Game::kScreenHeight = 15 * Game::kTileSize;

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING); // Initialize the SDL library. This must be called before using most other SDL functions.
	eventLoop();
}

Game::~Game() {
	SDL_Quit(); // Clean up all initialized subsystems. Call it upon all exit conditions.
}

void Game::eventLoop() {
	Graphics graphics; // Creates the window, renderer.
	Input input; // Key events.
	SDL_Event event; // A union that contains structures for the different event types.
	player_.reset(new Player(graphics, (kScreenWidth / 2), (kScreenHeight / 2))); // Resets what player_ is pointing to, copy-and-swap.
	map_.reset(Map::createTestMap(graphics));
	units::MS last_update_time = SDL_GetTicks();
	
	// This loop runs however many times kFps is set to per second. In this case 60 times per second.
	bool running = true;
	while (running) {
		const units::MS start_time_ms = SDL_GetTicks(); // SDL_GetTicks - get the number of milliseconds since the SDL library initialization.
		input.beginNewFrame(); // Clears pressed and released keys from the map.
		// HANDLE INPUT.
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.repeat == 0) { // Prevents being able to hold a key to repeat an action (such as jumping).
					input.keyDownEvent(event);
				}
				break;
			case SDL_KEYUP:
				input.keyUpEvent(event);
				break;
			default:
				break;
			}
		}

		// QUIT THE GAME.
		if (input.wasKeyPressed(SDLK_ESCAPE)) {
			running = false;
		}

		// HORIZONTAL MOVEMENT.
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

		if (input.isKeyHeld(SDLK_UP) && input.isKeyHeld(SDLK_DOWN)) {
			player_->lookHorizontal();
		}
		else if (input.isKeyHeld(SDLK_UP)) {
			player_->lookUp();
		}
		else if (input.isKeyHeld(SDLK_DOWN)) {
			player_->lookDown();
		}
		else {
			player_->lookHorizontal();
		}

		// JUMP.
		if (input.wasKeyPressed(SDLK_z)) {
			player_->startJump();
		}
		else if (input.wasKeyReleased(SDLK_z)) {
			player_->stopJump();
		}

		const units::MS current_time_ms = SDL_GetTicks();
		update(current_time_ms - last_update_time);
		last_update_time = current_time_ms;
		
		draw(graphics);
		
		// This is all to make sure the loop is running at the correct FPS.
		const units::MS ms_per_frame = (1000/*ms*/ / kFps);
		const units::MS elapsed_time_ms = SDL_GetTicks() - start_time_ms;
		if (elapsed_time_ms < ms_per_frame) {
			SDL_Delay(ms_per_frame - elapsed_time_ms); // Wait a specified number of milliseconds before returning.
		}

		/* TESTS THE EVENT LOOP BY CHECKING IF IT IS RUNNING AT THE CORRECT FPS.
		const float seconds_per_frame = (SDL_GetTicks() - start_time_ms) / 1000.0;
		const float fps = 1 / seconds_per_frame;
		printf("fps=%f\n", fps);
		*/
	}
}

// Move the player, move projecticles, and check collisions.
void Game::update(units::MS elapsed_time_ms) {
	player_->update(elapsed_time_ms, *map_);
	map_->update(elapsed_time_ms);
}

// Draw everything to the screen.
void Game::draw(Graphics& graphics) {
	graphics.clear();
	map_->drawBackground(graphics);
	player_->draw(graphics);
	map_->draw(graphics);
	graphics.flip();
}
