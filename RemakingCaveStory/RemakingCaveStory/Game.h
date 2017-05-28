#pragma once
#ifndef GAME_H_
#define GAME_H_

//Forward declaration
struct SDL_Window;
struct SDL_Renderer;

class Game {
private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;
	void eventLoop();
	void update();
	void draw();
public:
	Game();
	~Game();
};

#endif // !GAME_H_
