#ifndef VARYINGWIDTHSPRITE_H_
#define VARYINGWIDTHSPRITE_H_

#include "Sprite.h"

class VaryingWidthSprite : public Sprite
{
public:
	VaryingWidthSprite(Graphics& graphics,
					   const std::string& file_path,
					   units::Pixel source_x,
					   units::Pixel source_y,
					   units::Pixel initial_width,
					   units::Pixel initial_height) :
		Sprite(graphics, file_path, source_x, source_y, initial_width, initial_height) {}

	void setWidth(units::Pixel width) { source_rect_.w = width; }
private:
};

#endif // !VARYING_WIDTH_SPRITE_H_
