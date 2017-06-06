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
		units::Pixel source_x, units::Pixel source_y,
		units::Pixel width, units::Pixel height);
	virtual void update(units::MS) {} // Don't give parameter name because it gives unused variable warning.
	void draw(Graphics& graphics, units::Game x, units::Game y);
};

#endif // !SPRITE_H_
