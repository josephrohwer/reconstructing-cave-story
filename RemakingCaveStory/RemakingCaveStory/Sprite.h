#pragma once
#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL.h>
#include <string>
#include "Units.h"

class Graphics;

class Sprite {
private:
	SDL_Texture* sprite_sheet_;
protected: // So we can change it in the AnimatedSprite class.
	SDL_Rect source_rect_;
public:
	Sprite(Graphics& graphics, 
		const std::string& file_name,
		int source_x, int source_y, 
		int width, int height);
	virtual void update(units::MS) {} // Don't give parameter name because it gives unused variable warning.
	void draw(Graphics& graphics, int x, int y);
};

#endif // !SPRITE_H_
