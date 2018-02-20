#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL.h>
#include <string>
#include "Units.h"

class Graphics;

class Sprite
{
public:
	Sprite(Graphics& graphics,
		   const std::string& file_name,
		   units::Pixel source_x, units::Pixel source_y,
		   units::Pixel width, units::Pixel height);
	virtual void update(units::MS) {}
	void draw(Graphics& graphics, units::Game x, units::Game y);
protected:
	SDL_Rect source_rect_;
private:
	SDL_Texture* sprite_sheet_;
};

#endif // !SPRITE_H_
