#pragma once
#ifndef INPUT_H_
#define INPUT_H_

#include <SDL.h>
#include <map>

class Input {
private:
	std::map<SDL_Keycode, bool> held_keys_;
	std::map<SDL_Keycode, bool> pressed_keys_;
	std::map<SDL_Keycode, bool> released_keys_;
public:
	void beginNewFrame();
	void keyDownEvent(const SDL_Event& event);
	void keyUpEvent(const SDL_Event& event);
	bool wasKeyPressed(SDL_Keycode key);
	bool wasKeyReleased(SDL_Keycode key);
	bool isKeyHeld(SDL_Keycode key);
};

#endif // !INPUT_H_
