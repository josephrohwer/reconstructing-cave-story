#include <SDL_mixer.h>
#include "Game.h"
#include "Graphics.h"
#include "Audio.h"
#include "Player.h"
#include "Bat.h"
#include "Input.h"
#include "AnimatedSprite.h"
#include "Map.h"

// An anonymous namespace. Can only be accessed within this file.
namespace
{
	const units::FPS kFps = 60;
	const units::MS kMaxFrameTime = 5 * 1000 / 60;
}

// STATIC : ONLY ONE COPY OF THIS IS EVER MADE, NO MATTER HOW MANY OBJECTS ARE CREATED.
units::Tile Game::kScreenWidth = 20; // 20 * 32 = 640.
units::Tile Game::kScreenHeight = 15; // 15 * 32 = 480.

Game::Game()
{
	// Initialize the SDL library. This must be called before using most other SDL functions.
	SDL_Init(SDL_INIT_EVERYTHING);
}

Game::~Game()
{
	// Clean up all initialized subsystems. Call it upon all exit conditions.
	Mix_Quit();
	SDL_Quit();
}

void Game::eventLoop()
{
	// Creates the window, renderer.
	Graphics graphics;
	
	// Loads the audio.
	Audio audio;

	// Key events.
	Input input;

	// A union that contains structures for the different event types.
	SDL_Event event;
	
	// Resets what player_ is pointing to, copy-and-swap.
	player_.reset(new Player(graphics,
							 units::tileToGame((kScreenWidth / 2)), 
							 units::tileToGame((kScreenHeight / 2))));

	bat_.reset(new Bat(graphics,
					   units::tileToGame(5),
					   units::tileToGame((kScreenHeight / 2))));

	map_.reset(Map::createTestMap(graphics));

	Mix_PlayMusic(audio.main_music_, -1);

	// SDL_GetTicks - get the number of milliseconds since the SDL library initialization.
	units::MS last_update_time = SDL_GetTicks();
	
	// This loop runs however many times kFps is set to per second. In this case 60 times per second.
	bool running = true;
	while (running)
	{
		const units::MS start_time = SDL_GetTicks();
		
		// Clears pressed and released keys from the map.
		input.beginNewFrame();

		// HANDLE INPUT.
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					// Prevents being able to hold a key to repeat an action (such as jumping).
					if (!event.key.repeat)
					{
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
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
		{
			running = false;
		}

		// HORIZONTAL MOVEMENT.
		if (input.isKeyHeld(SDL_SCANCODE_LEFT) && input.isKeyHeld(SDL_SCANCODE_RIGHT))
		{
			player_->stopMoving();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT))
		{
			player_->startMovingLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT))
		{
			player_->startMovingRight();
		}
		else
		{
			player_->stopMoving();
		}

		// DIRECTION FACING.
		if (input.isKeyHeld(SDL_SCANCODE_UP) && input.isKeyHeld(SDL_SCANCODE_DOWN))
		{
			player_->lookHorizontal();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_UP)) 
		{
			player_->lookUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN))
		{
			player_->lookDown();
		}
		else
		{
			player_->lookHorizontal();
		}

		// JUMP.
		if (input.wasKeyPressed(SDL_SCANCODE_Z)) 
		{
			player_->startJump(audio.jump_sound_);
		}
		else if (input.wasKeyReleased(SDL_SCANCODE_Z))
		{
			player_->stopJump();
		}

		const units::MS current_time = SDL_GetTicks();
		const units::MS elapsed_time = current_time - last_update_time;

		// Move the player, move projecticles, and check collisions.
		update(std::min(elapsed_time, kMaxFrameTime));
		
		if (bat_->damageRectangle().collidesWith(player_->damageRectangle()))
		{
			player_->takeDamage(audio.hit_sound_);
		}

		last_update_time = current_time;
		
		// Draw everything to the screen.
		draw(graphics);
		
		// This is all to make sure the loop is running at the correct FPS.
		const units::MS ms_per_frame = (1000/*ms*/ / kFps);
		const units::MS time_elapsed = SDL_GetTicks() - start_time;

		if (time_elapsed < ms_per_frame)
		{
			// Wait a specified number of milliseconds before returning.
			SDL_Delay(ms_per_frame - time_elapsed);
		}
	}
}

void Game::update(units::MS elapsed_time)
{
	//map_->update(elapsed_time);
	player_->update(elapsed_time, *map_);
	bat_->update(elapsed_time, player_->center_x());
}

void Game::draw(Graphics& graphics)
{
	graphics.clear();

	map_->drawBackground(graphics);
	bat_->draw(graphics);
	player_->draw(graphics);
	map_->draw(graphics);
	player_->drawHUD(graphics);

	graphics.flip();
}
