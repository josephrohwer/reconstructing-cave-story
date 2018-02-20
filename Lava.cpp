#include "Lava.h"
#include "AnimatedSprite.h"
#include "Graphics.h"

Lava::Lava(Graphics& graphics, units::Game x, units::Game y) :
	x_(x),
	y_(y)
{
	initializeSprite(graphics);
}

void Lava::update(units::MS elapsed_time)
{
	//sprite_->update(elapsed_time);
}

void Lava::draw(Graphics& graphics) const
{
	sprite_->draw(graphics, x_, y_);
}

void Lava::initializeSprite(Graphics& graphics)
{
	sprite_ = boost::shared_ptr<Sprite>(new Sprite(graphics, "PrtCave",
		units::tileToPixel(13), units::tileToPixel(units::Tile(0)),
		units::tileToPixel(1), units::tileToPixel(1)));
}
